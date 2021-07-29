package sbgo

func pathInZigZagTree(label int) []int {
	row := 1
	for (1 << row) <= label {
		row++
	}
	getrv := func(row, label int) int {
		return (1 << row) + (1 << (row - 1)) - label - 1
	}
	if row%2 == 0 {
		label = getrv(row, label)
	}
	res := []int{}
	for row > 0 {
		if row%2 == 0 {
			res = append(res, getrv(row, label))
		} else {
			res = append(res, label)
		}
		label >>= 1
		row--
	}
	l, r := 0, len(res)-1
	for l < r {
		res[l], res[r] = res[r], res[l]
		l++
		r--
	}
	return res
}
