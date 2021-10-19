def get_time(path: str) -> int:
    with open(path, mode="r") as file:
        lines: list = file.read().split("\n")
        time: int = int(lines[1].split()[-2])
    return time


def convert(time: int) -> int:
    # Converts time (microseconds -> seconds)
    return time * (10 ** -6)


if __name__ == '__main__':
    parallel: int = round(
        convert(get_time("parallel.txt")), 3
    )
    print(f"Parallel time: {parallel} seconds")

    sequential: int = round(
        convert(get_time("sequential.txt")), 3
    )
    print(f"Sequential time: {sequential} seconds")
