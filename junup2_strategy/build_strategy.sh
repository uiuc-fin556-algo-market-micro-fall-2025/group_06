#!/bin/bash
cd "$HOME/ss/sdk/RCM/StrategyStudio/examples/strategies/"


#remove hard coded references in the checked out project to /home/vagrant vs now any user can run it from their own home directory (linux shortcut for your home dir is "~" FYI students)
sed -i.bak 's|/home/vagrant|~|g' ~/ss/sdk/RCM/StrategyStudio/examples/strategies/junup2_strategy/Makefile

cd junup2_strategy
mkdir -p "$HOME/ss/bt/strategies_dlls"
make copy_strategy

cd "$HOME/ss/bt/"
./StrategyServerBacktesting &

sleep 1

cd "$HOME/ss/bt/utilities"
./StrategyCommandLine cmd create_instance Junup2InstanceTwoStrategy Junup2Strategy UIUC SIM-1001-101 dlariviere 1000000 -symbols "UNH|GS|HD|MSFT|CRM|MCD|HON|BA|V|CAT|MMM|NKE|AXP|DIS|JPM|JNJ|TRV|AAPL|WMT|PG|IBM|CVX|MRK|DOW|CSCO|KO|VZ|INTC|WBA|KD"
./StrategyCommandLine cmd strategy_instance_list
./StrategyCommandLine cmd quit




