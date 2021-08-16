#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
 
#include <fcntl.h>
 
#define MP_ALIGNMENT 32
#define MP_PAGE_SIZE 4096
#define MP_MAX_ALLOC_FROM_POOL (MP+PAGE_SIZE - 1)

#define mp_align(n, alignment) (((n)+(alignment-1)) & ~(alignment-1))
#define mp_align_ptr(p, alignment) (void *)((((size_t)p)+(alignment-1)) & ~(alignment-1))


// 小块内存
struct mp_node_s{
    unsigned char* start;
    unsigned char* end;
    
    struct mp_node_s *next;
    int flag;
};

// 大块内存
struct mp_node_l{
    struct mp_node_l *next;
    void* alloc;
};


// 内存池结构体
struct mp_pool{
    size_t max;
    struct mp_node_s *cur;
    struct mp_node_l *large;

    // 柔性数组，一个标签的作用
    struct mp_node_s head[0];
};



// create pool just for init
struct mp_pool* mp_pool_create(size_t size){
    struct mp_pool *p;
    // 分配4k以上的就用这个函数，不用malloc,这里要注意加入这个头部p
    // 只有第一个节点是不一样的
    int ret = posix_memalign(&p,MP_ALIGNMENT,size + sizeof(struct mp_pool) + sizeof(struct mp_node_s));
    if(ret < 0) return NULL;
    
    // 4k大小
    p->max = size < MP_PAGE_SIZE ? size : MP_PAGE_SIZE;
    p->cur = p->head;
    p->large  = NULL;

    p->head->start = (unsigned char*)p + sizeof(struct mp_pool) + sizeof(struct mp_node_s);
    p->head->end = p->head->start + size;
    p->head->flag = 0;
    
    return p;
}


void mp_reset_pool(struct mp_pool *pool) {

	struct mp_node_s *h;
	struct mp_node_l *l;

	for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			free(l->alloc);
		}
	}

	pool->large = NULL;

	for (h = pool->head; h; h = h->next) {
		h->start = (unsigned char *)h + sizeof(struct mp_node_s);
	}

}


// destroy
void mp_destroy_pool(struct mp_pool* pool){
    struct mp_node_s *h,*n;
    struct mp_node_l *l;

    for(l = pool->large;l;l = l->next){
        if(l->alloc){
            free(l->alloc);
        }
    }
    
    h = pool->head->next;
    while(h){
        n = h->next;
        free(h);
        h = n;
    }

    free(pool);
}

// malloc/calloc 
// 小块
void* mp_alloc_block(struct mp_pool* pool,size_t size){
    
    unsigned char *m;
    struct mp_node_s *h = pool->head;
    size_t psize = (size_t)(h->end - (unsigned char*)h);
    m = NULL;
    // 分配的是一个区域
    int ret = posix_memalign(&m,MP_ALIGNMENT,psize + sizeof(struct mp_node_s));
    if(ret) return NULL;

    struct mp_node_s *p,*new_node,*current;
    new_node = (struct mp_node_s*)m;

    new_node->end = m + psize;
    new_node->next = NULL;
    new_node->flag = 0;

    m += sizeof(struct mp_node_s);
    m = mp_align_ptr(m,MP_ALIGNMENT);
    new_node->start = m + size;

    current = pool->cur;

    for(p = current;p->next;p = p->next){
        if(p->flag++ > 4){
            current = p->next;
        }
    }
    p->next = new_node;

    pool->cur = current?current:new_node;
    // new_node->next = pool->cur;
    // pool->cur = new_node;
    
    //pool->head[0] = m;
    
    return m;
}

// 大块
static void *mp_alloc_large(struct mp_pool *pool, size_t size) {
    void *p = malloc(size);
	if (p == NULL) return NULL;
    size_t n = 0;
    struct mp_node_l *large;
    for(large = pool->large;large;large = large ->next){
        if(large->alloc == NULL){
            large ->alloc = p;
            return p;
        }
        if(n++ > 3) break;
    }
    large = mp_alloc(pool,sizeof(struct mp_node_l));
    if(large == NULL){
        free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

void *mp_alloc(struct mp_pool *pool, size_t size) {
    unsigned char *m;
    struct mp_node_s *p;
    // 小块找到一块够用的内粗
    if(size <= pool ->max){
        p = pool -> cur;
        do{
            // m = p-> start;
            m = mp_align_ptr(p->start,MP_ALIGNMENT);
            if((size_t)(p->end - m) >= size){
                p->start = m + size;
                return m;
            }
            p = p->next;
        }while(p);
        return mp_alloc_block(pool,size);
    }
    return mp_alloc_large(pool,size);

}

void *mp_calloc(struct mp_pool *pool, size_t size) {

	void *p = mp_alloc(pool, size);
	if (p) {
		memset(p, 0, size);
	}

	return p;
}


// free

void mp_free(struct mp_pool *pool, void *p) {

	struct mp_node_l *l;
	for (l = pool->large; l; l = l->next) {
		if (p == l->alloc) {
			free(l->alloc);
			l->alloc = NULL;
			return ;
		}
	}
}

#if 0


#endif