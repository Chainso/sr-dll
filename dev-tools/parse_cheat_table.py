import json
from xml.etree import ElementTree
from typing import Dict, Tuple

def parse_cheat_entry(
        cheat_entry: ElementTree.Element,
        offset_count: int = 0,
        offset: str = "0"
    ) -> Tuple[str, Dict]:
    """
    Parses a single cheat entry into a tuple of the name and address
    information.

    Args:
        cheat_entry: The XML cheat entry.
        offset_count: The number of offsets to remove because they are
            duplicates of the parent node.
        offset: The offset of the first offset.
    
    Returns:
        A tuple, the first element being the name of the entry, and the second
        being the following dictionary:
        {
            "address": hex
            "offsets": [hex],
            "children": [dict]
        }
    """
    name = cheat_entry.find("Description").text.strip("\"").lower().replace(
        " ", "-"
    )
    address = cheat_entry.find("Address").text


    offsets_field = cheat_entry.find("Offsets")

    if offsets_field is not None:
        offsets = list(reversed([
            off.text for off in cheat_entry.find("Offsets").findall("Offset")
        ]))[offset_count:]
    else:
        offsets = []

    if not offsets:
        offsets = ["0"]

    base = 16
    offsets[0] = "{:x}".format(
        int(offsets[0], base) - int(offset, base)
    ).upper()

    children = {}

    child_nodes = cheat_entry.find("CheatEntries")
    if child_nodes:
        children = parse_cheat_entries(
            child_nodes, offset_count + len(offsets) - 1, offsets[-1]
        )

    entry_info = {
        "address": address,
        "offsets": offsets,
        "children": children
    }

    return name, entry_info


def parse_cheat_entries(
        cheat_entries: ElementTree.Element,
        offset_count: int = 0,
        offset: str = "0"
    ) -> Dict[str, Dict]:
    """
    Parses the cheat entries element in the XML tree.

    Args:
        cheat_entries: The cheat entries XML element.
        offset_count: The number of offsets to remove because they are
            duplicates of the parent node.
        offset: The offset of the first offset.

    Returns:
        A dictionary of the cheat names to their entries.
    """
    return dict(
        parse_cheat_entry(entry, offset_count, offset)
        for entry in cheat_entries
    )


def parse_cheat_table(cheat_table_file: str):
    """
    Reads the given cheat table and returns a dictionary of its contents.

    Args:
        cheat_table_file: The path to the XML cheat table.

    Returns:
        A dictionary of the cheat table contents
    """
    table_tree = ElementTree.parse(cheat_table_file)
    root = table_tree.getroot()

    cheat_entries = root.find("CheatEntries")

    return parse_cheat_entries(cheat_entries)

if __name__ == "__main__":
    reverse_engineering_path = "../reverse-engineering/"
    cheat_table_file = reverse_engineering_path + "SpeedRunners.CT"
    parsed_cheats_file = reverse_engineering_path + "speedrunners-structs.json"
    reclass_struct_file = reverse_engineering_path + "reclass-structs.json"

    cheat_table = parse_cheat_table(cheat_table_file)

    with open(parsed_cheats_file, "w") as cf:
        json.dump(cheat_table, cf, sort_keys=True, indent=2)

    with open(reclass_struct_file, "w") as rsf:
        for name, entry in cheat_table.items():
            rsf.write(name + "\n\n")

            address = entry["address"]

            for offset in entry["offsets"]:
                address = "[{}] + {}".format(address, offset)

            rsf.write(address + "\n\n\n")
