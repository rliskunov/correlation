def get_time(path: str) -> int:
    with open(path, mode="r") as file:
        lines: list = file.read().split("\n")
        time: int = int(lines[1].split()[-2])
    return time


def convert(time: int) -> int:
    # Converts time (microseconds -> seconds)
    return time * (10 ** -6)


if __name__ == '__main__':
    parallel: int = convert(get_time("parallel.txt"))
    print(f"Parallel time: {round(parallel, 3)} seconds")

    sequential: int = convert(get_time("sequential.txt"))
    print(f"Sequential time: {round(sequential, 3)} seconds")

    try:
        result = round(sequential / parallel, 3)
        print(f"Sequential/Parallel: {result}")
    except ZeroDivisionError as err:
        print(f"ZeroDivisionError: {err}")
