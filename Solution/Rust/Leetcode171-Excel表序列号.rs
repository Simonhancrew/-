impl Solution {
    pub fn title_to_number(column_title: String) -> i32 {
        let (mut res,mut n) = (0,column_title.len());
        for i in 0..n{
            res = res * 26 + (column_title.as_bytes()[i] - b'A' + 1) as i32;
        }
        res
    }
}