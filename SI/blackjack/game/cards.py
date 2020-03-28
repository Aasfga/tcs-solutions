from dataclasses import dataclass


@dataclass(frozen=True, eq=True)
class Card:
    value: int
    is_ace: bool = False
