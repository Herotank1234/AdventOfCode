use std::fs;

fn is_all_zeros(seq: &Vec<i32>) -> bool {
  for s in seq {
    if *s != 0 { return false; }
  }
  return true;
}

fn extrapolate(seq: &Vec<i32>) -> i32 {
  if is_all_zeros(seq) { return 0; }

  let diffs = seq.iter().clone().zip(seq[1..].iter().clone()).map(|(a, b)| b - a).collect::<Vec<i32>>();
  return seq.last().unwrap() + extrapolate(&diffs);
}

fn back_extrapolate(seq: &Vec<i32>) -> i32 {
  if is_all_zeros(seq) { return 0; }

  let diffs = seq.iter().clone().zip(seq[1..].iter().clone()).map(|(a, b)| b - a).collect::<Vec<i32>>();
  return seq.first().unwrap() - back_extrapolate(&diffs);
}

fn sum_extrapolated_values(seqs: &Vec<Vec<i32>>) -> i32 {
  return seqs.iter().map(|seq| extrapolate(seq)).sum();
}

fn sum_back_extrapolated_values(seqs: &Vec<Vec<i32>>) -> i32 {
  return seqs.iter().map(|seq| back_extrapolate(seq)).sum();
}

fn main() {
  let contents = fs::read_to_string("./data/q9.txt")
    .expect("Should have been able to read file");
  let lines = contents.split('\n').collect::<Vec<&str>>();
  let sequences = lines.iter().map(|line| line.split(' ').map(|s| s.parse::<i32>().unwrap()).collect::<Vec<i32>>())
    .collect::<Vec<Vec<i32>>>();

  println!("Part 1: {}", sum_extrapolated_values(&sequences));
  println!("Part 2: {}", sum_back_extrapolated_values(&sequences));
}