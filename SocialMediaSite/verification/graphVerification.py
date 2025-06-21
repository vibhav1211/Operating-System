import csv
from collections import defaultdict

# --- Load graph (skip header line) ---
graph = defaultdict(set)
with open('musae_git_edges.csv', 'r') as f:
    next(f)  # Skip the header line: id_1,id_2
    for line in f:
        src, dst = map(int, line.strip().split(','))
        graph[src].add(dst)
        graph[dst].add(src)  # because the graph is undirected

# --- Load generated actions ---
generated = defaultdict(lambda: [0, 0, 0])  # POST, COMMENT, LIKE
with open('verification/generated_actions.csv', 'r') as f:
    reader = csv.reader(f)
    next(reader)  # Skip header
    for row in reader:
        if row[0] == 'Total':
            continue  # skip the summary row
        uid = int(row[0])
        generated[uid] = list(map(int, row[1:]))

# --- Load read actions ---
read = defaultdict(lambda: [0, 0, 0])
with open('verification/read_actions.csv', 'r') as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        if row[0] == 'Total':
            continue  # skip the summary row
        uid = int(row[0])
        read[uid] = list(map(int, row[1:]))

# --- Verify and log mismatches ---
tot = 0  # counter for correct mappings

with open('verification/result.log', 'w') as log:
    log.write("UserID | MISMATCH\n")
    log.write("-------|---------\n")

    for user in range(0, 37700):
        neighbors = graph[user]

        # Sum neighbor generated actions
        neighbor_gen_sum = [0, 0, 0]  # POST, COMMENT, LIKE
        for neighbor in neighbors:
            neighbor_actions = generated[neighbor]
            for i in range(3):
                neighbor_gen_sum[i] += neighbor_actions[i]

        user_read = read[user]

        if user_read != neighbor_gen_sum:
            log.write(f"{user:6d} | Read: {user_read} vs NeighborSum: {neighbor_gen_sum}\n")
        else:
            tot += 1

    # Log total correct mappings at the end
    log.write(f"\nTotal correct mappings: {tot}\n")