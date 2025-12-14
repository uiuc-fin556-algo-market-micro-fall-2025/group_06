#!/bin/bash

# Function to show usage and exit
usage() {
    echo "Usage: $0 --start-date YYYY-MM-DD --end-date YYYY-MM-DD"
    exit 1
}

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --start-date)
            startDate="$2"
            shift 2
            ;;
        --end-date)
            endDate="$2"
            shift 2
            ;;
        *)
            echo "Unknown parameter: $1"
            usage
            ;;
    esac
done

# Validate that both arguments are provided
if [[ -z "$startDate" || -z "$endDate" ]]; then
    echo "Error: both --start-date and --end-date are required."
    usage
fi

# Validate date format (YYYY-MM-DD)
if ! [[ "$startDate" =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ && "$endDate" =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ ]]; then
    echo "Error: Dates must be in YYYY-MM-DD format."
    usage
fi

instanceName='Junup2InstanceTwoStrategy'

(cd "$HOME/ss/bt" && ./StrategyServerBacktesting &)

echo "Sleeping for 2 seconds while waiting for strategy studio to boot"
sleep 2

# Start the backtest
(cd "$HOME/ss/bt/utilities" && ./StrategyCommandLine cmd start_backtest "$startDate" "$endDate" "$instanceName" 0)

# Get the line number which ends with finished.
foundFinishedLogFile=$(grep -nr "finished.$" "$HOME/ss/bt/logs/main_log.txt" | gawk '{print $1}' FS=":" | tail -1)

# DEBUGGING OUTPUT
echo "Last line found: $foundFinishedLogFile"

# Wait for strategy to finish
while ((logFileNumLines > foundFinishedLogFile)); do
    foundFinishedLogFile=$(grep -nr "finished.$" "$HOME/ss/bt/logs/main_log.txt" | gawk '{print $1}' FS=":" | tail -1)
    echo "Waiting for strategy to finish"
    sleep 1
done

echo "Sleeping for 10 seconds..."
sleep 10

echo "run_backtest.sh: Strategy Studio finished backtesting"

