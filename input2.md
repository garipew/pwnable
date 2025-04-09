## input2
This level is all about input.

## Solution
### Stage 1
In the first stage of this challenge, we have to pass 99 values (the program name is counted on argc) as command line arguments, this can easily be done with
```bash
./input2 $(printf "a %.0s" {1..99})
```
Since we need some special values, for now on the 65 and 66 arguments, we can modify it a little to 
```bash
./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' $(printf "a %.0s" {67..99})
```
Stage 1 clear!
### Stage 2
In this stage we have to write on both stdin and stderr, this can be achieved with redirects. But we have to pay attention, since both inputs require to pass the null byte.
```bash
printf "\x00\x0a\x00\xff" | ./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' $(printf "a %.0s" {67..99}) 2< <(echo -n -e "\x00\x0a\x02\xff")
``` 
Stage 2 clear!
### Stage 3
This time we need to set an enviroment variable, '\xde\xad\xbe\xef', with the value of '\xca\xfe\xba\xbe'. To set env variables in bash, we can use the syntax
```bash
env name=value
```
Since both the name and the value contains non-printable characters, we can set it with
```bash
env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe'
```
We can even go further, by declaring the enviroment variable exclusively to a exist while executing something, with
```bash
env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe' command
```
With everything together, we get
```bash
printf "\x00\x0a\x00\xff" | env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe' ./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' $(printf "a %.0s" {67..99}) 2< <(echo -n -e "\x00\x0a\x02\xff")
```
Stage 3 clear!
### Stage 4
In this stage, the program tries to read from a file named '\x0a', we can achive this by first creating a temporary directory with
```bash
mktemp -d
```
There, we can both create the file and write to it, very easily
```bash
touch $'\x0a'
echo -n -e "\x00\x00\x00\x00" > $'\x0a'
```
Then, we should execute the binary from our repository, by doing either, the more straight forward way
```bash
printf "\x00\x0a\x00\xff" | env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe' $HOME/input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' $(printf "a %.0s" {67..99}) 2< <(echo -n -e "\x00\x0a\x02\xff")
```
Or, by symlinking the file to our directory, and since doing that, we should probably symlink the flag too
```bash
ln -sf $HOME/input2 input2
ln -sf $HOME/flag flag
```
And then executing with our usual
```bash
printf "\x00\x0a\x00\xff" | env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe' ./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' $(printf "a %.0s" {67..99}) 2< <(echo -n -e "\x00\x0a\x02\xff")
```
Stage 4 clear!
### Stage 5
Our last stage, at last! First things first, we need to go back to the first stage, another argument is special, so we should give a more meaninful value to it
```bash
./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' "6666" $(printf "a %.0s" {68..99})
```
This stage is about sending values via network, the new argument we gave is a port that the program will open and read from, so what we need to do is to connect to this port and send the expected message '\xde\xad\xbe\xef'.<br>
In total we have
```bash
printf "\x00\x0a\x00\xff" | env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe' ./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' "6666" $(printf "a %.0s" {68..99}) 2< <(echo -n -e "\x00\x0a\x02\xff")
```
This time, we want to execute in the background though, so we are able to send the message without interference, we achieve this by adding **&** at the end
```bash
printf "\x00\x0a\x00\xff" | env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe' ./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' "6666" $(printf "a %.0s" {68..99}) 2< <(echo -n -e "\x00\x0a\x02\xff") &
```
Now, we can easily send the message using nc, with
```bash
echo -e "\xde\xad\xbe\xef" | nc 0 6666
```
Stage 5 clear!<br>
Success!
