use std::fs;
use std::collections::HashMap;

#[derive(Debug, PartialEq)]
enum JSONData {
  JSONArray(Vec<JSONData>),
  JSONObject(HashMap<String, JSONData>),
  JSONString(String),
  JSONNumber(i32),
}

fn parse_key(string: &Vec<char>, index: usize) -> (usize, String) {
  let mut curr_index = index;
  let mut str = String::new();
  curr_index += 1;
  while string[curr_index] != '\"' {
    str.push(string[curr_index]);
    curr_index += 1;
  }
  return (curr_index + 1, str);
}

fn parse_json(string: &Vec<char>, index: usize) -> (usize, JSONData) {
  let mut curr_index = index;
  match string[curr_index] {
    '{' => {
      let mut key_values: HashMap<String, JSONData> = HashMap::new();
      curr_index += 1;
      while string[curr_index] != '}' {
        let (next_index, key) = parse_key(string, curr_index);
        curr_index = next_index + 1;
        let (after_index, value) = parse_json(string, curr_index);
        key_values.insert(key, value);
        curr_index = after_index;
        if string[curr_index] == ',' { curr_index += 1 }
      }
      return (curr_index + 1, JSONData::JSONObject(key_values));
    },

    '[' => {
      let mut values: Vec<JSONData> = Vec::new();
      curr_index += 1;
      while string[curr_index] != ']' {
        let (next_index, value) = parse_json(string, curr_index);
        values.push(value);
        curr_index = next_index;
        if string[curr_index] == ',' { curr_index += 1 }
      }
      return (curr_index + 1, JSONData::JSONArray(values));
    },

    '\"' => {
      let mut json_string = String::new();
      curr_index += 1;
      while string[curr_index] != '\"' {
        json_string.push(string[curr_index]);
        curr_index += 1;
      }
      return (curr_index + 1, JSONData::JSONString(json_string));
    }

    _ => {
      let mut json_value = String::new();
      while (string[curr_index] == '-') || (string[curr_index] >= '0' && string[curr_index] <= '9') {
        json_value.push(string[curr_index]);
        curr_index += 1;
      }
      return (curr_index, JSONData::JSONNumber(json_value.parse::<i32>().unwrap()));
    }
  }
}

fn sum_numbers(json_data: &JSONData) -> i32 {
  match json_data {
    JSONData::JSONString(_) => return 0,

    JSONData::JSONNumber(val) => return *val,

    JSONData::JSONArray(vec) => 
      return vec.iter().map(|data| sum_numbers(data)).sum(),

    JSONData::JSONObject(map) =>
      return map.iter().map(|(_, v)| sum_numbers(v)).sum(),
  }
}

fn sum_numbers_ignore_red(json_data: &JSONData) -> i32 {
  match json_data {
    JSONData::JSONString(_) => return 0,

    JSONData::JSONNumber(val) => return *val,

    JSONData::JSONArray(vec) => 
      return vec.iter().map(|data| sum_numbers_ignore_red(data)).sum(),

    JSONData::JSONObject(map) => {
      let mut sum = 0;
      for (_, v) in map.iter() {
        if *v == JSONData::JSONString("red".to_string()) {
          return 0;
        } else {
          sum += sum_numbers_ignore_red(v);
        }
      }
      return sum;
    }
  }
}

fn main() {
  let document = fs::read_to_string("./data/q12.txt")
    .expect("Should have been able to read file");

  let (_, json_data) = parse_json(&document.chars().collect::<Vec<char>>(), 0);
  
  println!("Part 1: {}", sum_numbers(&json_data));
  println!("Part 2: {}", sum_numbers_ignore_red(&json_data));
}