use std::env;
use std::fs::File;
use std::io::{self, BufRead};
use std::process::exit;

fn read_file_to_grid(file_path: &str) -> Result<Vec<Vec<char>>, io::Error> {
    File::open(file_path)
        .map(io::BufReader::new)
        .and_then(|reader: io::BufReader<File>| {
            reader.lines().collect::<Result<Vec<String>, io::Error>>()
        })
        .map(|lines: Vec<String>| {
            lines
                .into_iter()
                .map(|line| line.chars().collect())
                .collect()
        })
}

fn wormhole_empty_space(grid: &mut Vec<Vec<char>>) {
    grid.first()
        .map(|l| l.len())
        .map(|line_len| (0..line_len).rev())
        .map(|cols| {
            let indices_to_change: Vec<_> = cols
                .filter(|&col| grid.iter().all(|l| l[col] == '.'))
                .collect();

            indices_to_change.into_iter().for_each(|col| {
                grid.iter_mut().for_each(|line| line[col] = 'o');
            });
        });
    grid.iter_mut()
        .filter(|v| v.iter().all(|&c| c == '.' || c == 'o'))
        .for_each(|v| v.iter_mut().for_each(|c| *c = 'o'));
}

fn find_galaxy_positions(grid: &Vec<Vec<char>>) -> Vec<(u128, u128)> {
    grid.iter()
        .enumerate()
        .flat_map(|(i, row)| {
            row.iter().enumerate().filter_map(move |(j, &ch)| {
                if ch == '#' {
                    Some((u128::try_from(i).unwrap(), u128::try_from(j).unwrap()))
                } else {
                    None
                }
            })
        })
        .collect()
}

fn wormhole_galaxy_positions(
    grid: &Vec<Vec<char>>,
    galaxy_positions: &mut Vec<(u128, u128)>,
    wormhole_length: u128,
) {
    for (x, y) in galaxy_positions {
        let mut new_position: (u128, u128) = (0, 0);
        for i in 0..usize::try_from(*x).unwrap() {
            match grid[i][0] {
                'o' => new_position.0 += wormhole_length,
                _ => new_position.0 += 1,
            }
        }
        for i in 0..usize::try_from(*y).unwrap() {
            match grid[0][i] {
                'o' => new_position.1 += wormhole_length,
                _ => new_position.1 += 1,
            }
        }
        *x = new_position.0;
        *y = new_position.1;
    }
}

fn shortest_path(from: &(u128, u128), too: &(u128, u128)) -> u128 {
    from.0.abs_diff(too.0) + from.1.abs_diff(too.1)
}

fn sum_galaxy_paths(galaxy_positions: &Vec<(u128, u128)>) -> u128 {
    galaxy_positions
        .iter()
        .flat_map(|&from| {
            galaxy_positions
                .iter()
                .map(move |&too| shortest_path(&from, &too))
        })
        .sum()
}

fn get_galaxy_path_sums(grid: &mut Vec<Vec<char>>, wormhole_length: u128) -> u128 {
    wormhole_empty_space(grid);

    let mut galaxies = find_galaxy_positions(&grid);
    wormhole_galaxy_positions(&grid, &mut galaxies, wormhole_length);

    sum_galaxy_paths(&galaxies) / 2
}

fn main() -> Result<(), io::Error> {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        println!("Give input file as argument.");
        exit(0);
    }

    let mut grid: Vec<Vec<char>> = read_file_to_grid(args.get(1).unwrap())?;

    let sum = get_galaxy_path_sums(&mut grid, 1_000_000);

    println!("Sum of galaxy paths: {sum}");

    Ok(())
}
