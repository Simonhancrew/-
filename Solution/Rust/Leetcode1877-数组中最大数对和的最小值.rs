impl Solution {
    pub fn min_pair_sum(mut nums: Vec<i32>) -> i32 {
        nums.sort_unstable();
        let mut mm = 0;
        let (mut l,mut r) = (0,nums.len() - 1);
        while l < r{
            mm = mm.max(nums[l] + nums[r]);
            r -= 1;
            l += 1;
        }
        mm as i32
    }
}