import System.IO

readToLists :: FilePath -> IO [[Int]]
readToLists filePath = do
    content <- readFile filePath
    let numLines = lines content
    return $ map (map read . words) numLines

test :: [Int] -> Bool
test xs = all (`elem` [1..3]) xs || all (`elem` [(-3)..(-1)]) xs

withoutOne :: [a] -> [[a]]
withoutOne xs = [take i xs ++ drop (i + 1) xs | i <- [0..length xs - 1]]

main :: IO ()
main = do
    lists <- readToLists "input"
    let pairs = map (\x -> zip x (tail x)) lists
    let diffs = map (\z -> map(\(x, y) -> x-y) z) pairs

    let perms = map (withoutOne) lists
    let ppairs = map (map (\x -> zip x (tail x))) perms
    let ddiffs = map (map (\z -> map(\(x, y) -> x-y) z)) ppairs

    putStrLn $ "Result part 1: " ++ show (length (filter (test) diffs))
    putStrLn $ "Result part 2: " ++ show (length $ filter (not . null) (map (filter (test)) ddiffs))

