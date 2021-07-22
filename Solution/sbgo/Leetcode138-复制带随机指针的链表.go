package sbgo

func copyRandomList(head *Node) *Node {
	dummy := &Node{Val: -1}
	mp := map[*Node]*Node{}
	tmp, nh := head, dummy
	for tmp != nil {
		nh.Next = &Node{Val: tmp.Val}
		nh = nh.Next
		mp[tmp] = nh
		tmp = tmp.Next
	}
	tmp, nh = head, dummy.Next
	for tmp != nil {
		if tmp.Random != nil {
			nh.Random = mp[tmp.Random]
		}
		tmp = tmp.Next
		nh = nh.Next
	}
	return dummy.Next
}
