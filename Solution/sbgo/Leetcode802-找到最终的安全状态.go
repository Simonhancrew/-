package sbgo

func eventualSafeNodes(graph [][]int) []int {
	n := len(graph)
	rev := make([][]int, n)
	d := make([]int, n)
	for i := 0; i < n; i++ {
		for _, b := range graph[i] {
			a := i
			rev[b] = append(rev[b], a)
			d[a]++
		}
	}
	que := []int{}
	for i := 0; i < n; i++ {
		if d[i] == 0 {
			que = append(que, i)
		}
	}
	for len(que) > 0 {
		t := que[0]
		que = que[1:]
		for _, b := range rev[t] {
			d[b]--
			if d[b] == 0 {
				que = append(que, b)
			}
		}
	}
	res := []int{}
	for i := 0; i < n; i++ {
		if d[i] == 0 {
			res = append(res, i)
		}
	}
	return res
}
