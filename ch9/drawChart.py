from matplotlib import pyplot as plt
import csv

joblen = []
fairness = []

with open('fairness.txt', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter='\t')
    for row in plots:
        joblen.append(float(row[0]))
        fairness.append(float(row[1]))
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
plt.ylim(0.0,1.005)
plt.xlim(1,1000)
ax.set_xscale('log')
ax.plot(joblen, fairness, 'y')
plt.xlabel('Job Length')
plt.ylabel('Unfairness')
plt.title('Lottery Fairness Study')
# auxiliary line
ax.axhline(y=1, linewidth=1, color='b', linestyle='--')

plt.savefig('Lottery Fairness Study.png')
