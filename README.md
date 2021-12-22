# Command Line Verb Lookup (CLVL)

CLVL is a command line tool to lookup verbs and verb conjugations without leaving your terminal.  

CLVL comes with a data file for looking up Spanish verbs (data/jehle_verb_database.csv) but you can also use your own csv formatted file to lookup verbs in other languages.

## Usage

`clvl <verb> <tense>`

Example:

```shell
./clvl comer

comer to eat Indicativo Presente I eat, am eating comiendo comido
como comes come comemos coméis comen

./clvl comer "Futuro"

comer to eat Indicativo Futuro I will eat comiendo comido
comeré comerás comerá comeremos comeréis comerán
```

#### Help

```shell
clvl --help

Usage: clvl verb 
   or: clvl verb [OPTION]...

Search for a Spanish verb and return the conjugation in English.
Buscar un verbo en español y devolver la conjugación en inglés.

Example:  clvl comer
Example2: clvl comer "Futuro perfecto"
Example3: clvl comer "Futuro perfecto" --file ../data/my_conjugation_file.csv 

OPTIONS:
  -h [ --help ]                         Help screen
  -f [ --file ] arg (=../data/jehle_verb_database.csv)
                                        
                                        A CSV formatted data file to parse. 
                                        Defaults to ../data/jehle_verb_database
                                        .csv
  -t [ --tense ] arg (=Presente)        
                                        The tense with which to conjugate the 
                                        verb.  Defaults to 'Presente'.
                                        Tenses are as follows:
                                        "Presente"
                                        "Futuro"
                                        "Imperfecto"
                                        "Pretérito"
                                        "Condicional"
                                        "Presente perfecto"
                                        "Futuro perfecto"
                                        "Pluscuamperfecto"
                                        "Pretérito anterior"
                                        "Condicional perfecto"
                                        "Indicative"
                                        
                                        
  -v [ --verb ] arg                     
                                        A Spanish verb in infinitive form e.g. 
                                        'comer' to be conjugated.

```

## Installation

### Ubuntu  

Given the following platform:

```shell
Ubuntu 20.04.3 LTS (GNU/Linux 5.11.0-1020-aws x86_64)

uname -a
Linux 5.11.0-1020-aws #21~20.04.2-Ubuntu SMP Fri Oct 1 13:03:59 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux
```

#### Pre-requisites:

Install pre-requisite packages:
- build-essentials (g++)
- clang-14
- libbboost-all-dev

Install g++ and supporting tools:

```shell
sudo apt-get install build-essential
```

Install clang:

```shell
sudo apt-get install clang-14
```

Install boost libraries:

```shell
sudo apt-get install libboost-all-dev
```

#### Build

##### Clang

###### Clone and compile the project:

```shell
git clone https://github.com/s-michael/clvl.git
cd clvl
mkdir bin
cd main

clang++-14 -O2 -Wall  main.cpp -o ../bin/clvl -lboost_iostreams -lboost_filesystem -lboost_program_options 
```
 
###### Run

```shell
cd ../bin

./clvl comer

```

###### Symlink

```shell
sudo ln -s clvl ../usr/bin/clvl
```

##### Bazel

clvl can be build with [Bazel](https://bazel.build/).
[Install Bazel](https://docs.bazel.build/versions/main/install.html)

__Note__: the toolchain/cc_toolchain_config.bzl file may need to be updated to reflect your system's build tools and locations.
__todo__: make the toolchain info less static

###### Clone and build the project:

```shell
git clone https://github.com/s-michael/clvl.git
cd clvl
bazel build --config=clang_config  //main:clvl
```

###### Run

```shell
./bazel-bin/main/clvl comer Futuro Subjuntivo -f data/jehle_verb_database.csv
```
__Note__: Bazel will pull in dependencies (the boost libraries). 

