if __name__ == '__main__':
    with open("result.txt") as lines:
        line = lines.read().split("\n")
        parallel_time = int(line[1].split()[-2])
        non_parallel_time = int(line[3].split()[-2])

    result: str = f"The execution time cannot be counted"
    try:
        times: float = round(parallel_time / non_parallel_time, 3)
        result: str = f"The execution time of parallel calculations is {times} times less"
        print(result)
    except ZeroDivisionError as err:
        print(f"Error: {err}")

    with open("result.txt", mode="a") as file:
        file.write(result)
