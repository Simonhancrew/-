package sbgo

func minOperations(target []int, arr []int) int {
	mp := map[int]int{}
	for i := 0; i < len(target); i++ {
		mp[target[i]] = i
	}
	n := len(target)
	d := []int{}
	for _, val := range arr {
		if idx, has := mp[val]; has {
			if len(d) == 0 || d[len(d)-1] < idx {
				d = append(d, idx)
			} else {
				l, r := 0, len(d)-1
				for l < r {
					mid := (l + r) >> 1
					if d[mid] >= idx {
						r = mid
					} else {
						l = mid + 1
					}
				}
				d[l] = idx
			}
		}
	}
	return n - len(d)
}
