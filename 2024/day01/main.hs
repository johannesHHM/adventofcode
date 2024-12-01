import System.IO
import Data.List

readToLists :: FilePath -> IO ([Int], [Int])
readToLists path = fmap (foldr (\line (l1, l2) -> let [x, y] = map read (words line) in (x:l1, y:l2)) ([], [])) (lines <$> readFile path)

count :: Eq a => a -> [a] -> Int
count x xs = length (filter (== x) xs)

main :: IO ()
main = do
    (list1, list2) <- readToLists "input"

    let res1 = sum [ abs (x - y) | (x, y) <- zip (sort list1) (sort list2) ]
    let res2 = sum [ x * (count x list2) | x <- list1 ]
    putStrLn $ "Result part 1: " ++ show res1
    putStrLn $ "Result part 2: " ++ show res2
