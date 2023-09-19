use std::fs;

trait Packet {
  fn get_version_total(&self) -> u32;
  fn evaluate(&self) -> u64;
}

struct LiteralPacket {
  version: u32,
  _id: u32,
  value: u64
}

impl LiteralPacket {
  fn new(version: u32, id: u32, value: u64) -> Self {
    return Self { version, _id: id, value };
  }
}

impl Packet for LiteralPacket {
  fn get_version_total(&self) -> u32 {
    return self.version;
  }

  fn evaluate(&self) -> u64 {
    return self.value;
  }
}

struct OperatorPacket {
  version: u32,
  id: u32,
  sub_packets: Vec<Box<dyn Packet>>
}

impl OperatorPacket {
  fn new(version: u32, id: u32, sub_packets: Vec<Box<dyn Packet>>) -> Self {
    return Self { version, id, sub_packets };
  }
}

impl Packet for OperatorPacket {
  fn get_version_total(&self) -> u32 {
    return self.version + self.sub_packets.iter()
      .fold(0, |acc, packet| acc + packet.get_version_total());
  }

  fn evaluate(&self) -> u64 {
    let evals: Vec<u64> = self.sub_packets.iter()
      .map(|packet| packet.evaluate()).collect();
    let result: u64 = match self.id {
      0 => evals.iter().sum::<u64>(),
      1 => evals.iter().product::<u64>(),
      2 => *evals.iter().min().unwrap(),
      3 => *evals.iter().max().unwrap(),
      5 => (evals[0] > evals[1]) as u64,
      6 => (evals[0] < evals[1]) as u64,
      7 => (evals[0] == evals[1]) as u64,
      _ => 0,
    };
    return result;
  }
}

fn parse_to_packet(binary: &Vec<char>, index: usize) -> (usize, Box<dyn Packet>) {
  let mut curr_index = index;

  let version_str: String = binary[curr_index..curr_index + 3].iter().collect();
  let version = u32::from_str_radix(&version_str, 2).unwrap();
  curr_index += 3;

  let id_str: String = binary[curr_index..curr_index + 3].iter().collect();
  let id = u32::from_str_radix(&id_str, 2).unwrap();
  curr_index += 3;

  if id == 4 {
    let mut value_str = String::new();
    while binary[curr_index] == '1' {
      curr_index += 1;
      let next_value: String = binary[curr_index..curr_index + 4].iter().collect();
      value_str.insert_str(value_str.len(), &next_value);
      curr_index += 4;
    }

    curr_index += 1;
      let next_value: String = binary[curr_index..curr_index + 4].iter().collect();
    value_str.insert_str(value_str.len(), &next_value);
    curr_index += 4;

    return (curr_index, Box::new(LiteralPacket::new(version, id, 
      u64::from_str_radix(&value_str, 2).unwrap())));
  } else {
    let length_type_id_str = binary[curr_index];
    let mut sub_packets: Vec<Box<dyn Packet>> = Vec::new();

    curr_index += 1;
    if length_type_id_str == '0' {
      let sub_packet_len_str: String = binary[curr_index..curr_index + 15].iter().collect();
      let sub_packet_len = usize::from_str_radix(&sub_packet_len_str, 2).unwrap();
      curr_index += 15;
      
      let end_index = curr_index + sub_packet_len;

      while curr_index < end_index {
        let (next_index, sub_packet) = parse_to_packet(binary, curr_index);
        sub_packets.push(sub_packet);
        curr_index = next_index;
      }
    } else {
      let num_sub_packets_str: String = binary[curr_index..curr_index + 11].iter().collect();
      let num_sub_packets = u32::from_str_radix(&num_sub_packets_str, 2).unwrap();
      curr_index += 11;
      
      for _ in 0..num_sub_packets {
        let (next_index, sub_packet) = parse_to_packet(binary, curr_index);
        sub_packets.push(sub_packet);
        curr_index = next_index;
      }
    }
    return (curr_index, Box::new(OperatorPacket::new(version, id, sub_packets)));
  }
}

fn parse_to_binary(contents: &str) -> Vec<char> {
  let binary: String = contents.chars()
    .fold("".to_string() ,|acc,hex| {
      let next_bin = match hex {
        '0' => "0000".to_string(),
        '1' => "0001".to_string(),
        '2' => "0010".to_string(),
        '3' => "0011".to_string(),
        '4' => "0100".to_string(),
        '5' => "0101".to_string(),
        '6' => "0110".to_string(),
        '7' => "0111".to_string(),
        '8' => "1000".to_string(),
        '9' => "1001".to_string(),
        'A' => "1010".to_string(),
        'B' => "1011".to_string(),
        'C' => "1100".to_string(),
        'D' => "1101".to_string(),
        'E' => "1110".to_string(),
        'F' => "1111".to_string(),
        _ => "".to_string()
      };
      return acc + next_bin.as_str();
    });
  return binary.chars().collect();
}

fn main() {
  let contents = fs::read_to_string("./data/q16.txt")
    .expect("Should have been able to read file");
  let binary = parse_to_binary(&contents);
  let (_, packet) = parse_to_packet(&binary, 0);

  println!("Part 1: {}", packet.get_version_total());
  println!("Part 2: {}", packet.evaluate());
}