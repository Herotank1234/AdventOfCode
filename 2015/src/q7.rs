use std::fs;
use std::collections::HashMap;

fn evaluate<'a>(memo: &mut HashMap<&'a str, u16>, gates: &HashMap<&str, Vec<&'a str>>, wire_name: &'a str) -> u16 {
  if memo.contains_key(&wire_name) {
    return *memo.get(&wire_name).unwrap();
  }

  let gate = gates.get(wire_name).unwrap();
  let result: u16 = match gate.len() {
    2 =>{
      let value = gate[0].parse::<u16>();
      match value {
        Ok(val) => val,
        Err(_) => evaluate(memo, gates, gate[0])
      }
    },

    3 => !evaluate(memo, gates, gate[1]),

    4 => {
      let value1 = gate[0].parse::<u16>();
      let src1 = match value1 {
        Ok(val) => val,
        Err(_) => evaluate(memo, gates, gate[0])
      };

      let value2 = gate[2].parse::<u16>();
      let src2 = match value2 {
        Ok(val) => val,
        Err(_) => evaluate(memo, gates, gate[2])
      };

      match gate[1] {
        "AND" => src1 & src2,
        "OR" => src1 | src2,
        "LSHIFT" => src1 << src2,
        "RSHIFT" => src1 >> src2,
        _ => {
          println!("No such instruction: {}", gate[1]);
          0
        }
      }
    },

    _ => {
      println!("No match for gate length: {}, gate: {:?}", gate.len(), gate);
      0
    }
  };

  memo.insert(&wire_name, result);

  return result;
}

fn evaluate_wire_signal(gates: &HashMap<&str, Vec<&str>>, wire_name: &str) -> u16 {
  let mut memo: HashMap<&str, u16> = HashMap::new();
  return evaluate(&mut memo, gates, wire_name);
}

fn evaluate_wire_signal_twice(gates: &HashMap<&str, Vec<&str>>, wire_name: &str, wire_update: &str) -> u16 {
  let value = evaluate_wire_signal(gates, wire_name).to_string();
  let value_next = value.as_str();

  let mut gates_next = gates.clone();
  *gates_next.get_mut(wire_update).unwrap() = vec![value_next, wire_update];
  return evaluate_wire_signal(&gates_next, wire_name)
}

fn main() {
  let contents = fs::read_to_string("./data/q7.txt")
    .expect("Should have been able to read file");

   let gates: HashMap<&str, Vec<&str>> = contents
    .split("\n")
    .map(|gate_str| 
      {
        let components = gate_str.split(" ")
          .filter(|component| *component != "->")
          .collect::<Vec<&str>>();
        (*components.last().unwrap(), components)
      }
    )
    .collect::<HashMap<&str, Vec<&str>>>();
  println!("Part 1: {}", evaluate_wire_signal(&gates, "a"));
  println!("Part 2: {}", evaluate_wire_signal_twice(&gates, "a", "b"));
}