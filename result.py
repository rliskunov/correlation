def get_time(path: str) -> float:
    with open(path, mode="r") as file:
        lines: list = file.read().split("\n")
        time: float = float(lines[1].split()[-2])
    return time


if __name__ == '__main__':
    parallel: float = get_time("parallel.txt")
    print(f"Parallel time: {round(parallel, 3)} seconds")

    sequential: float = get_time("sequential.txt")
    print(f"Sequential time: {round(sequential, 3)} seconds")

    try:
        result = round(sequential / parallel, 3)
        print(f"Sequential/Parallel: {result}")
    except ZeroDivisionError as err:
        print(f"ZeroDivisionError: {err}")
