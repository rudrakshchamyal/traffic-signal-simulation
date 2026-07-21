import pandas as pd
import matplotlib.pyplot as plt
import os

# 1. Configuration and Setup
CSV_FILE = 'simulation_results.csv'

# Check if the C++ output file exists
if not os.path.exists(CSV_FILE):
    print(f"Error: Could not find '{CSV_FILE}'.")
    print("Please make sure you run the C++ program first to generate the data!")
    exit()

# 2. Load the Data
print(f"Loading data from {CSV_FILE}...")
df = pd.read_csv(CSV_FILE)

# 3. Plotting Setup
# We will use an academic style suitable for a research report
plt.style.use('seaborn-v0_8-whitegrid')

# ---------------------------------------------------------
# GRAPH 1: Cycle Length vs. Traffic Flow
# ---------------------------------------------------------
plt.figure(figsize=(10, 6))

# Plot Fuzzy Logic line (Blue with circle markers)
plt.plot(df['Flow'], df['Fuzzy_Cycle_Length'], 
         marker='o', linestyle='-', color='#1f77b4', linewidth=2, 
         label='Fuzzy Logic Controller')

# Plot Webster line (Red with square markers)
plt.plot(df['Flow'], df['Webster_Cycle_Length'], 
         marker='s', linestyle='--', color='#d62728', linewidth=2, 
         label="Webster's Formula (Fixed Time)")

# Formatting Graph 1
plt.title('Traffic Signal Cycle Length vs. Traffic Flow', fontsize=16, fontweight='bold')
plt.xlabel('Traffic Flow (vehicles/hour)', fontsize=14)
plt.ylabel('Cycle Length (seconds)', fontsize=14)
plt.legend(fontsize=12, loc='upper left')
plt.grid(True, linestyle=':', alpha=0.7)
plt.tight_layout()

# Save Graph 1 as a high-res image for the report
output_file_1 = 'Flow_vs_CycleLength.png'
plt.savefig(output_file_1, dpi=300)
print(f"Successfully generated: {output_file_1}")

# ---------------------------------------------------------
# GRAPH 2: Cycle Length vs. Traffic Density
# ---------------------------------------------------------
plt.figure(figsize=(10, 6))

# Plot Fuzzy Logic line
plt.plot(df['Density'], df['Fuzzy_Cycle_Length'], 
         marker='o', linestyle='-', color='#2ca02c', linewidth=2, 
         label='Fuzzy Logic Controller')

# Plot Webster line
plt.plot(df['Density'], df['Webster_Cycle_Length'], 
         marker='s', linestyle='--', color='#ff7f0e', linewidth=2, 
         label="Webster's Formula (Fixed Time)")

# Formatting Graph 2
plt.title('Traffic Signal Cycle Length vs. Traffic Density', fontsize=16, fontweight='bold')
plt.xlabel('Traffic Density (vehicles/km)', fontsize=14)
plt.ylabel('Cycle Length (seconds)', fontsize=14)
plt.legend(fontsize=12, loc='upper left')
plt.grid(True, linestyle=':', alpha=0.7)
plt.tight_layout()

# Save Graph 2 as a high-res image for the report
output_file_2 = 'Density_vs_CycleLength.png'
plt.savefig(output_file_2, dpi=300)
print(f"Successfully generated: {output_file_2}")

# Display the plots on the screen
plt.show()