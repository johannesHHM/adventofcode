use std::{
    env,
    fs::File,
    io::{self, BufRead, BufReader},
    process::exit,
    str::SplitAsciiWhitespace,
};

fn get_integer(line: &String) -> u64 {
    return line
        .split(":")
        .nth(1)
        .unwrap()
        .chars()
        .filter(|c| !c.is_whitespace())
        .collect::<String>()
        .parse::<u64>()
        .unwrap();
}

fn get_integers(line: &String) -> SplitAsciiWhitespace<'_> {
    return line.split(":").nth(1).unwrap().split_ascii_whitespace();
}

fn test_numbers(time: u64, dest: u64) -> u32 {
    let mut count: u32 = 0;
    for i in 1..time {
        if (time - i) * i > dest {
            count += 1;
        }
    }
    count
}

fn findfirst(time_line: &String, dest_line: &String) -> u32 {
    let time_dest_iterator = get_integers(&time_line)
        .zip(get_integers(&dest_line))
        .filter(|(t, d)| t.parse::<u32>().is_ok() && d.parse::<u32>().is_ok())
        .map(|(t, d)| (t.parse::<u32>().unwrap(), d.parse::<u32>().unwrap()));

    let mut sum: u32 = 1;
    for (time, dest) in time_dest_iterator {
        sum *= test_numbers(time as u64, dest as u64);
    }
    sum
}

fn findsecond(time_line: &String, dest_line: &String) -> u32 {
    let time: u64 = get_integer(time_line);
    let dest: u64 = get_integer(dest_line);

    let mut count: u32 = 0;
    count += test_numbers(time, dest);
    count
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Give input file as argument.");
        exit(0);
    }

    let file: File = File::open(args.get(1).ok_or("Could not open file.")?)?;
    let mut lines: io::Lines<BufReader<File>> = BufReader::new(file).lines();

    let time_line = lines.next().ok_or("Could not parse timeline")??;
    let dest_line = lines.next().ok_or("Could not parse destination line")??;

    let sum_first: u32 = findfirst(&time_line, &dest_line);
    let sum_second: u32 = findsecond(&time_line, &dest_line);

    println!("sum: {sum_first}");
    println!("sum: {sum_second}");

    Ok(())
}
