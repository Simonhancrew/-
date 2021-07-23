package sbgo

func isCovered(ranges [][]int, left int, right int) bool {
	df := make([]int, 52)
	for i := range ranges {
		df[ranges[i][0]]++
		df[ranges[i][1]+1]--
	}

	cur := 0
	for i := 1; i <= 50; i++ {
		cur += df[i]
		if i >= left && i <= right && cur <= 0 {
			return false
		}
	}
	return true
}
