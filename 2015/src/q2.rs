use std::fs;

fn calculate_wrapping_paper(dimensions: &Vec<Vec<u32>>) -> u32 {
  let paper_needed:u32 = dimensions.iter().fold(0, |acc, dim| {
    let side1: u32 = dim[0] * dim[1];
    let side2: u32 = dim[1] * dim[2];
    let side3: u32 = dim[2] * dim[0];
    let sides: Vec<u32> = vec![side1, side2, side3];
    let slack: &u32 = sides.iter().min().unwrap();
    acc + 2 * side1 + 2 * side2 + 2 * side3 + slack
  });
  return paper_needed;
}

fn calculate_ribbon(dimensions: &Vec<Vec<u32>>) -> u32 {
  let ribbon_needed: u32 = dimensions.iter().fold(0, |acc, dim| {
    let perim1: u32 = 2 * dim[0] + 2 * dim[1];
    let perim2: u32 = 2 * dim[1] + 2 * dim[2];
    let perim3: u32 = 2 * dim[2] + 2 * dim[0];
    let bow = dim[0] * dim[1] * dim[2];
    let perims = vec![perim1, perim2, perim3];
    let smallest_perim = perims.iter().min().unwrap();
    acc + smallest_perim + bow
  });
  return ribbon_needed;
}

fn main() {
  let contents = fs::read_to_string("./data/q2.txt")
    .expect("Should have been able to read file");
  let dimensions: Vec<Vec<u32>> = contents.split("\n")
    .map(|dimension: &str|
      dimension.split("x")
        .map(|side: &str| side.parse::<u32>().unwrap())
        .collect::<Vec<u32>>()
    )
    .collect::<Vec<Vec<u32>>>();

  println!("Part 1: {}", calculate_wrapping_paper(&dimensions));
  println!("Part 2: {}", calculate_ribbon(&dimensions));
}