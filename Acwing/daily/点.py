n = int(input())

# 经典推公式，可以化简

a,b = [],[]
for _ in range(n):
    x,y = map(int,input().split())
    a.append(x)
    b.append(y)
    
def get(arr):
    res = 0
    res += n * sum([x *x for x in arr])
    res -= sum(arr) * sum(arr)
    return res      
print(get(a) + get(b))