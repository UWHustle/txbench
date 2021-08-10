machine_id=$(echo $HOSTNAME | tr -dc '0-9')
outfolder="${1}_${machine_id}_runtxbench"
mkdir -p $outfolder

sizes="
  100000
  1000000
  10000000
"

workers="
 1
 4
 8
 10
"

times="
 10
 60
"

for s in ${sizes}; do
  for w in ${workers}; do
    for t in ${times}; do
    outfile=${1}"_txbench_ART_"${s}"_"${w}"_"${t}
    ./build/src/inmemoryarrays/inmemarrays_ART --num_rows ${s} --num_workers ${w} --measure_duration ${t} 2>&1 >"${outfolder}/${outfile}_wip" && mv "${outfolder}/${outfile}_wip" "${outfolder}/${outfile}" &
  done
done

for s in ${sizes}; do
  for w in ${workers}; do
    for t in ${times}; do
    outfile=${1}"_txbench_ALEX_"${s}"_"${w}"_"${t}
    ./build/src/inmemoryarrays/inmemarrays_ALEX --num_rows ${s} --num_workers ${w} --measure_duration ${t} 2>&1 >"${outfolder}/${outfile}_wip" && mv "${outfolder}/${outfile}_wip" "${outfolder}/${outfile}" &
  done
done

for s in ${sizes}; do
  for w in ${workers}; do
    for t in ${times}; do
    outfile=${1}"_txbench_BW_"${s}"_"${w}"_"${t}
    ./build/src/inmemoryarrays/inmemarrays_BW --num_rows ${s} --num_workers ${w} --measure_duration ${t} 2>&1 >"${outfolder}/${outfile}_wip" && mv "${outfolder}/${outfile}_wip" "${outfolder}/${outfile}" &
  done
done
