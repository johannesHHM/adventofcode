use std::{
    collections::HashMap,
    env,
    fs::File,
    io::{self, BufRead, BufReader},
    process::exit,
};

fn parse_card_first(card: String) -> u32 {
    let card: Vec<String> = card.split("|").map(|s| s.to_string()).collect();

    let winning_numbers: Vec<u32> = card
        .get(0)
        .unwrap()
        .split(" ")
        .filter(|s| s.parse::<u32>().is_ok())
        .map(|s| s.parse::<u32>().unwrap())
        .collect();

    let your_numbers: Vec<u32> = card
        .get(1)
        .unwrap()
        .split(" ")
        .filter(|s| s.parse::<u32>().is_ok())
        .map(|s| s.parse::<u32>().unwrap())
        .collect();

    let hits: usize = winning_numbers
        .iter()
        .filter(|i| your_numbers.contains(i))
        .count();

    hits.try_into().unwrap()
}

fn parse_cards_second(reader: BufReader<File>) -> u32 {
    let cards: Vec<String> = reader.lines().filter_map(|s| s.ok()).collect();

    let mut card_counts: HashMap<u32, u32> = cards
        .iter()
        .enumerate()
        .map(|(i, _)| ((i + 1).try_into().unwrap(), 1_u32))
        .into_iter()
        .collect();

    let mut card_id: u32 = 1;

    for line in cards {
        let hits: u32 = parse_card_first(line);
        let current_card_count: u32 = card_counts.get(&card_id).unwrap().clone();
        for i in 1..hits + 1 {
            *card_counts.get_mut(&(card_id + i)).unwrap() += current_card_count;
        }
        card_id += 1;
    }

    card_counts.iter().map(|(_, v)| v).sum()
}

fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Give input file path as argument.");
        exit(0);
    }

    let file: File = File::open(args.get(1).unwrap())?;
    let reader_first: BufReader<File> = BufReader::new(file);

    let mut card_sum: u32 = 0;

    for line in reader_first.lines() {
        let line: String = line?;
        let hits: u32 = parse_card_first(line);
        if hits > 0 {
            card_sum += 2_u32.pow(hits - 1);
        };
    }

    println!("Card sum first: {}", card_sum);

    let file: File = File::open(args.get(1).unwrap())?;
    let reader_second: BufReader<File> = BufReader::new(file);

    println!("Card count second: {}", parse_cards_second(reader_second));

    Ok(())
}
