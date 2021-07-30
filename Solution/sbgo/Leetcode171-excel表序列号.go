package sbgo

func titleToNumber(columnTitle string) int {
	res, n := 0, len(columnTitle)
	for i := 0; i < n; i++ {
		res = res*26 + (int)(columnTitle[i]-'A'+1)
	}
	return res
}
