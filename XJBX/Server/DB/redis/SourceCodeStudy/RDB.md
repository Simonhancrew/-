## RDB持久化

redis本质是个内存数据库，为了持久化数据到磁盘，提供了RDB持久化的功能。

数据库状态和RDB文件是可以互相转化的

### 创建和载入

有两个命令可以创建，SAVE和BGSAVE

save会阻塞服务器进程，知道RDB创建完成为止。

BG就是background，BGSAVE会派生出一个子进程，然后子进程去负责创建RDB文件。注意看一下rdb.c里的rdbSave函数做的事

```
/* Save the DB on disk. Return REDIS_ERR on error, REDIS_OK on success 
 *
 * 将数据库保存到磁盘上。
 *
 * 保存成功返回 REDIS_OK ，出错/失败返回 REDIS_ERR 。
 */
int rdbSave(char *filename) {
    dictIterator *di = NULL;
    dictEntry *de;
    char tmpfile[256];
    char magic[10];
    int j;
    long long now = mstime();
    FILE *fp;
    rio rdb;
    uint64_t cksum;

    // 创建临时文件
    snprintf(tmpfile,256,"temp-%d.rdb", (int) getpid());
    fp = fopen(tmpfile,"w");
    if (!fp) {
        redisLog(REDIS_WARNING, "Failed opening .rdb for saving: %s",
            strerror(errno));
        return REDIS_ERR;
    }

    // 初始化 I/O
    rioInitWithFile(&rdb,fp);

    // 设置校验和函数
    if (server.rdb_checksum)
        rdb.update_cksum = rioGenericUpdateChecksum;

    // 写入 RDB 版本号
    snprintf(magic,sizeof(magic),"REDIS%04d",REDIS_RDB_VERSION);
    if (rdbWriteRaw(&rdb,magic,9) == -1) goto werr;

    // 遍历所有数据库
    for (j = 0; j < server.dbnum; j++) {

        // 指向数据库
        redisDb *db = server.db+j;

        // 指向数据库键空间
        dict *d = db->dict;

        // 跳过空数据库
        if (dictSize(d) == 0) continue;

        // 创建键空间迭代器
        di = dictGetSafeIterator(d);
        if (!di) {
            fclose(fp);
            return REDIS_ERR;
        }

        /* Write the SELECT DB opcode 
         *
         * 写入 DB 选择器
         */
        if (rdbSaveType(&rdb,REDIS_RDB_OPCODE_SELECTDB) == -1) goto werr;
        if (rdbSaveLen(&rdb,j) == -1) goto werr;

        /* Iterate this DB writing every entry 
         *
         * 遍历数据库，并写入每个键值对的数据
         */
        while((de = dictNext(di)) != NULL) {
            sds keystr = dictGetKey(de);
            robj key, *o = dictGetVal(de);
            long long expire;
            
            // 根据 keystr ，在栈中创建一个 key 对象
            initStaticStringObject(key,keystr);

            // 获取键的过期时间
            expire = getExpire(db,&key);

            // 保存键值对数据
            if (rdbSaveKeyValuePair(&rdb,&key,o,expire,now) == -1) goto werr;
        }
        dictReleaseIterator(di);
    }
    di = NULL; /* So that we don't release it again on error. */

    /* EOF opcode 
     *
     * 写入 EOF 代码
     */
    if (rdbSaveType(&rdb,REDIS_RDB_OPCODE_EOF) == -1) goto werr;

    /* CRC64 checksum. It will be zero if checksum computation is disabled, the
     * loading code skips the check in this case. 
     *
     * CRC64 校验和。
     *
     * 如果校验和功能已关闭，那么 rdb.cksum 将为 0 ，
     * 在这种情况下， RDB 载入时会跳过校验和检查。
     */
    cksum = rdb.cksum;
    memrev64ifbe(&cksum);
    rioWrite(&rdb,&cksum,8);

    /* Make sure data will not remain on the OS's output buffers */
    // 冲洗缓存，确保数据已写入磁盘
    if (fflush(fp) == EOF) goto werr;
    if (fsync(fileno(fp)) == -1) goto werr;
    if (fclose(fp) == EOF) goto werr;

    /* Use RENAME to make sure the DB file is changed atomically only
     * if the generate DB file is ok. 
     *
     * 使用 RENAME ，原子性地对临时文件进行改名，覆盖原来的 RDB 文件。
     */
    if (rename(tmpfile,filename) == -1) {
        redisLog(REDIS_WARNING,"Error moving temp DB file on the final destination: %s", strerror(errno));
        unlink(tmpfile);
        return REDIS_ERR;
    }

    // 写入完成，打印日志
    redisLog(REDIS_NOTICE,"DB saved on disk");

    // 清零数据库脏状态
    server.dirty = 0;

    // 记录最后一次完成 SAVE 的时间
    server.lastsave = time(NULL);

    // 记录最后一次执行 SAVE 的状态
    server.lastbgsave_status = REDIS_OK;

    return REDIS_OK;

werr:
    // 关闭文件
    fclose(fp);
    // 删除文件
    unlink(tmpfile);

    redisLog(REDIS_WARNING,"Write error saving DB on disk: %s", strerror(errno));

    if (di) dictReleaseIterator(di);

    return REDIS_ERR;
}
```

RDB文件的载入时启动的时候自动进行的，所以不会有显式的命令。另外，AOF持久化的优先级是比RDB高的，所以优先会使用AOF持久化文件载入，只有AOF被关闭的时候才会使用RDB

### 自动间隔保存

save的选项值可以指定多个保存的条件，比如

```
save 900 1
```

900秒之内至少修改了一次，BGSAVE就会被执行。

在启动的时候没有设置参数的化，save选项会被设置为默认条件

之前redis.h中的redisServer中的

```
struct saveparam *saveparams;   /* Save points array for RDB */
```

会被设置成为保存的选项条件。

里面描述的属性

```
// 服务器的保存条件（BGSAVE 自动执行的条件）
struct saveparam {

    // 多少秒之内
    time_t seconds;

    // 发生多少次修改
    int changes;

};
```

然后redisServer中还和RDB相关的有两个字段

```
// 自从上次 SAVE 执行以来，数据库被修改的次数
long long dirty;                /* Changes to DB from the last save */
// 最后一次完成 SAVE 的时间
time_t lastsave;                /* Unix time of last successful save */
```

这个周期性的检测是由serverCron默认100ms执行一次的，其中一项工作就是检查是否满足save选项的条件

### RDB文件结构

| REDIS | db_version | databases | EOF  | check_sun |
| :---: | :--------: | :-------: | :--: | :-------: |

最开头是：REDIS，5个字符，一个检测的依据

db_version记录了版本号

databases可能保存了多个非空的数据库，

每个非空的数据库都可以表示成：

| SELECTDB | db_number | key_value_pairs |
| :------: | :-------: | :-------------: |

过期部分的k-v可能也会被保存到最后的部分中。

其中过期和不过其的k-v对是不一样的

不带过期的是由:

+ type记录了value的类型
+ key，一个字符串对象
+ value

带过期时间的是由：

+ expiretime_ms:一个标志
+ ms：unix时间戳，代表一个过期时间
+ type
+ key
+ value

### 关于value的编码

字符串对象的话，编码形式可以是REDIS_ENCODING_INT或者REDIS_ENCODING_RAW。如果是前者说明保存的是长度不超过32位的整数。

| ENCODING | integer |
| :------: | :-----: |

前者的encoding可以是int8，int6,int32.而关于raw形式的话，说明对象保存的是一个字符串值，长度小于20的不会压缩，大于20的会被压缩。

| REDIS_RDB_ENC_LZF | compressed_len | origin_len | compressed_sring |
| :---------------: | :------------: | :--------: | :--------------: |

第一个参数代表已经被LZF压缩算法压缩过了。

列表对象的结构

| list_length | item1 | item2 | ..... | itemn |
| :---------: | :---: | :---: | :---: | :---: |

哈希和集合和列表对象的结构类似，只不过哈希的k-v对中每个k-v前面有字符串对象的长度的一个空间

### 总结

+ 注意bgsave和save对服务器的影响
+ 可选条件保存，这样在满足的时候会自动执行BGsave
+ RDB文件是一个经过压缩的二进制文件
+ 不同的键值对，会用不同的方式来保存
