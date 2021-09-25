def method_name(path: str) -> int:
    with open(path) as lines:
        line = lines.read().split("\n")[1]
    return int(line.split()[-2])


if __name__ == '__main__':
    parallel_time: int = method_name("parallel_correlation.txt")
    non_parallel_time: int = method_name("non_parallel_correlation.txt")
    times: float = round(parallel_time / non_parallel_time, 3)

    result: str = f"The execution time of parallel calculations is {times} times less"
    coefficient: int = 1_000_000_000
    print(f"The parallel time: {round(parallel_time / coefficient, 5)} seconds")
    print(f"The nonparallel time: {round(non_parallel_time / coefficient, 5)} seconds")
    print(result)
    with open("results.txt", mode="w+") as file:
        file.write(result)
