impl Solution {
    pub fn is_covered(ranges: Vec<Vec<i32>>, left: i32, right: i32) -> bool {
        let mut df = vec![0;52];
        for r in ranges{
            df[r[0] as usize] += 1;
            df[r[1] as usize+ 1] -= 1;
        }
        let mut cur = 0;
        for i in 1..=50{
            cur += df[i];
            if i as i32 >= left && i as i32 <= right && cur <= 0{
                return false;
            }
        }
        true
    }
}