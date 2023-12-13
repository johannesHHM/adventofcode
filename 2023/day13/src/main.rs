use std::env;
use std::fs::File;
use std::io::{self, BufRead};
use std::process::exit;

fn read_grids_from_file(file_path: &str) -> Result<Vec<Vec<Vec<char>>>, io::Error> {
    let file = File::open(file_path)?;
    let reader = io::BufReader::new(file);

    let mut grids = Vec::new();
    let mut grid = Vec::new();

    for line in reader.lines() {
        let line = line?;

        if line.trim().is_empty() {
            grids.push(grid);
            grid = Vec::new();
        } else {
            let row: Vec<char> = line.trim().chars().collect();
            grid.push(row);
        }
    }
    if !grid.is_empty() {
        grids.push(grid);
    }

    Ok(grids)
}

fn equal_verticals(grid: &Vec<Vec<char>>, x0: usize, x1: usize) -> usize {
    let mut diff_count: usize = 0;
    for y in 0..grid.len() {
        if grid[y][x0] != grid[y][x1] {
            diff_count += 1;
        }
    }
    diff_count
}

fn equal_horizontal(grid: &Vec<Vec<char>>, y0: usize, y1: usize) -> usize {
    let mut diff_count: usize = 0;
    for x in 0..grid[0].len() {
        if grid[y0][x] != grid[y1][x] {
            diff_count += 1;
        }
    }
    diff_count
}

fn find_reflect_vertical(grid: &Vec<Vec<char>>, leeway: usize) -> i64 {
    for p in 0..grid[0].len() - 1 {
        let mut differances: usize = 0;
        for i in 0..=p {
            let mi = i + 2 * (p - i) + 1;
            if mi >= grid[0].len() {
                continue;
            }
            differances += equal_verticals(grid, i, mi);
        }
        if differances == leeway {
            return (p + 1).try_into().unwrap();
        }
    }
    return 0;
}

fn find_reflect_horizontal(grid: &Vec<Vec<char>>, leeway: usize) -> i64 {
    for p in 0..grid.len() - 1 {
        let mut differances: usize = 0;
        for i in 0..=p {
            let mi = i + 2 * (p - i) + 1;
            if mi >= grid.len() {
                continue;
            }
            differances += equal_horizontal(grid, i, mi);
        }
        if differances == leeway {
            return (p + 1).try_into().unwrap();
        }
    }
    return 0;
}

fn run(grids: &Vec<Vec<Vec<char>>>, leeway: usize) -> i64 {
    let mut sum_vertical: i64 = 0;
    let mut sum_horizontal: i64 = 0;

    for grid in grids.iter() {
        let mut p: i64;
        p = find_reflect_vertical(grid, leeway);
        if p != 0 {
            sum_vertical += p;
        } else {
            p = find_reflect_horizontal(grid, leeway);
            if p != 0 {
                sum_horizontal += p;
            }
        }
    }
    sum_vertical + 100 * sum_horizontal
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        println!("Give input file as argument.");
        exit(0);
    }

    let grids = read_grids_from_file(args.get(1).unwrap()).unwrap();

    println!("Result part 1: {}", run(&grids, 0));
    println!("Result part 2: {}", run(&grids, 1));
}
