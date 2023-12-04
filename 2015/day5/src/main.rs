use std::fs::File;
use std::io::{self, prelude::*, BufReader};
use std::env;
use std::process::exit;
use std::str;

const ILLEGAL_SUBSTRINGS: &[&str] = &["ab", "cd", "pq", "xy"];
const VOWELS: &[char] = &['a', 'e', 'i', 'o', 'u'];

fn check_name_second(name: &String) ->bool {
    let has_repeat: bool = name
        .as_bytes()
        .windows(2)
        .enumerate()
        .any(|(i, a)| name[i+2..].contains(unsafe { str::from_utf8_unchecked(a) }));
    
    if !has_repeat {
        return false;
    };

    let has_mirrored: bool = name
        .as_bytes()
        .windows(3)
        .any(|a| a[0] == a[2]);

    return has_mirrored;
}

fn check_name_first(name: &String) -> bool {
    let vowel_count: usize = name
        .to_ascii_lowercase()
        .chars()
        .filter(|c| VOWELS.contains(c))
        .count();
    
    if vowel_count < 3 {
        return false;
    };

    let has_double: bool = name
        .as_bytes()
        .windows(2)
        .any(|a| a[0] == a[1]);
    
    if !has_double {
        return false;
    };
    
    let has_illegal: bool = ILLEGAL_SUBSTRINGS
        .iter()
        .any(|s| name.contains(s));
    
    return !has_illegal;
}

fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Give input file path as argument.");
        exit(0);
    }

    let file: File = File::open(args.get(1).unwrap())?;
    let reader: BufReader<File> = BufReader::new(file);

    let mut first_count: u32 = 0;
    let mut second_count: u32 = 0;

    for line in reader.lines() {
        let line: String = line?;
        if check_name_first(&line) {
            first_count += 1;
        }
        if check_name_second(&line) {
            second_count += 1;
        }

    }
    println!("Nice names first count: {}", first_count);
    println!("Nice names second count: {}", second_count);

    Ok(())
}
