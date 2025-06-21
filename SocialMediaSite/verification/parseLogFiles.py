import csv
from collections import defaultdict

# Initialize dictionaries
generated = defaultdict(lambda: defaultdict(int))
read = defaultdict(lambda: defaultdict(int))

# Action types
action_types = ['POST', 'COMMENT', 'LIKE']

# Totals
total_generated = defaultdict(int)
total_read = defaultdict(int)

# Parse userActivity.log
with open('userActivity.log', 'r') as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) < 6:
            continue
        user_id = int(parts[1])
        action_info = parts[3]
        action_type = action_info.split('(')[0]
        if action_type in action_types:
            generated[user_id][action_type] += 1
            total_generated[action_type] += 1

# Parse readActivity.log
with open('readActivity.log', 'r') as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) < 10:
            continue
        user_id = int(parts[1])
        action_info = parts[3]
        action_type = action_info.split('(')[0]
        if action_type in action_types:
            read[user_id][action_type] += 1
            total_read[action_type] += 1


# # Write CSV for generated actions
# with open('verification/generated_actions.csv', 'w', newline='') as csvfile:
#     writer = csv.writer(csvfile)
#     writer.writerow(['UserId'] + action_types)
#     for user in range(0, 37700):
#         writer.writerow([user] + [generated[user][t] for t in action_types])

# Initialize total counter
total_all_actions = 0

# Write CSV for generated actions
with open('verification/generated_actions.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['UserId'] + action_types)

    for user in range(0, 37700):
        row = [generated[user][t] for t in action_types]
        total_all_actions += sum(row)
        writer.writerow([user] + row)

    # Append total at the end
    writer.writerow(['Total'] + [''] * (len(action_types) - 1) + [total_all_actions])

# Initialize total counter
total_read_actions = 0

# Write CSV for read actions
with open('verification/read_actions.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['UserId'] + action_types)

    for user in range(0, 37700):
        row = [read[user][t] for t in action_types]
        total_read_actions += sum(row)
        writer.writerow([user] + row)

    # Append total at the end
    writer.writerow(['Total'] + [''] * (len(action_types) - 1) + [total_read_actions])

