import uproot
import numpy as np
import awkward as ak
import argparse


genPath="/w/work/clas12/singh/HighLevelAN/outputs/simulations/rga/rga_spring2018/dvcsgen/new_inbeding_0na_10594/fulldata/Gen_Spring2018_dvcs_0na_inbend_full_data.root"
recPath="/w/work/clas12/singh/HighLevelAN/outputs/simulations/rga/rga_spring2018/dvcsgen/new_inbeding_0na_10594/fulldata/Recon_Spring2018_dvcs_0na_inbend_full_data.root"

# Set up argument parser
parser = argparse.ArgumentParser(description="Process ROOT files for gen/rec event matching.")

parser.add_argument("--genPath", type=str, default=genPath, help="Path to the generated ROOT file")
parser.add_argument("--recPath", type=str, default=recPath, help="Path to the reconstructed ROOT file")
parser.add_argument("--createTest", action="store_true", help="Flag to create a test set")
parser.add_argument("--endName", type=str, default="", help="Suffix to append to output filename")

args = parser.parse_args()

# Access values like this:
genPath = args.genPath
recPath = args.recPath
createTest = args.createTest
endName = args.endName

print('Opening Files')

# Step 1: Load the variables from the two ROOT files
gen_file = uproot.open(genPath)  # replace with your file name
rec_file = uproot.open(recPath)  # replace with your file name

gen_tree = gen_file["PhysicsEvents"]  # adjust the tree name if different
rec_tree = rec_file["PhysicsEvents"]

variables = ["num_events", "e_p", "e_theta", "e_phi", "p1_p", "p1_theta", "p1_phi", "p2_p", "p2_theta", "p2_phi"]

gen_data = gen_tree.arrays(variables, library="np")
rec_data = rec_tree.arrays(variables, library="np")

# Convert to structured arrays for ease of access
gen = np.column_stack([gen_data[var] for var in variables])
rec = np.column_stack([rec_data[var] for var in variables])

print('Got gen array',gen.shape)
print('Got rec array',rec.shape)

print('Take Only 5M events')

gen_mask=gen[:,0]<5000000
rec_mask=rec[:,0]<5000000

if createTest==True:
  gen_mask=(gen[:,0]>5000000) & (gen[:,0]<10000000)
  rec_mask=(rec[:,0]>5000000) & (rec[:,0]<10000000)
  endName=endName+"_test"

gen=gen[gen_mask]
rec=rec[rec_mask]

print('gen array',gen.shape)
print('rec array',rec.shape)

# Create dictionaries for quick lookup by evnum
gen_dict = {int(evnum): vals for evnum, vals in zip(gen[:, 0], gen)}

print('Converted to dic')

# Step 2: Augment rec with the corresponding gen event values
rec_augmented = []
for row in rec:
    evnum = int(row[0])
    if evnum in gen_dict:
        true_vals = gen_dict[evnum][1:]  # skip evnum
        rec_row = np.concatenate([row[1:], true_vals, [1]])  # skip evnum, add 1 for accepted
        rec_augmented.append(rec_row)

print('Edited Rec')

# Stack original gen array (without evnum), pad with zeros for true values, and accepted = 0
gen_augmented = []
for row in gen:
    gen_row = np.concatenate([[0]*9, row[1:], [0]])  # no rec values, add gen values, accepted = 0
    gen_augmented.append(gen_row)

print('Edited Gen')

rec_augmented = np.array(rec_augmented)
gen_augmented = np.array(gen_augmented)

# Step 3: Combine arrays and write to a ROOT file
all_data = np.vstack([rec_augmented, gen_augmented])

print('Shuffle Data')
indices = np.random.permutation(len(all_data))
X_shuffled = all_data[indices]

# Define branch names
branch_names = [
    "recel_p", "recel_theta", "recel_phi",
    "recp1_p", "recp1_theta", "recp1_phi",
    "recp2_p", "recp2_theta", "recp2_phi",
    "truel_p", "truel_theta", "truel_phi",
    "trup1_p", "trup1_theta", "trup1_phi",
    "trup2_p", "trup2_theta", "trup2_phi",
    "accepted"
]

print('Writing out dataset',all_data.shape)

# Write to a new ROOT file
with uproot.recreate("all_events"+endName+".root") as f:
    f["tree"] = {name: all_data[:, i] for i, name in enumerate(branch_names)}
