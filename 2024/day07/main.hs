import System.IO
import Data.List (subsequences)

parseLine :: String -> (Int, [Int])
parseLine line = 
    let (keyStr:vals) = words line
        key = read (init keyStr) :: Int
        values = map read vals :: [Int]
    in (key, values)

parseFile :: FilePath -> IO [(Int, [Int])]
parseFile filePath = do
    contents <- readFile filePath
    let linesList = lines contents
    return (map parseLine linesList)

catInts :: Int -> Int -> Int
catInts a b = read (show a ++ show b) :: Int

sumGiven :: Int -> [Int] -> [Char] -> Int
sumGiven sum [] [] = sum
sumGiven sum (x:xs) (op:ops)
    | op == '+' = sumGiven (sum+x) xs ops
    | op == '*' = sumGiven (sum*x) xs ops
    | op == '|' = sumGiven (catInts sum x) xs ops
    | otherwise = 0

allSums :: [Int] -> [Char] -> [Int]
allSums (x:xs) ops = [sumGiven x xs ops | ops <- sequence (replicate (length xs) ops)]

main :: IO ()
main = do
    tuples <- parseFile "input"
    let res1 = sum $ map (\(x, y) -> x) (filter (\(x, y) -> elem x (allSums y "+*")) tuples)
    let res2 = sum $ map (\(x, y) -> x) (filter (\(x, y) -> elem x (allSums y "+*|")) tuples)
    putStrLn $ "Result part 1: " ++ show res1
    putStrLn $ "Result part 2: " ++ show res2

