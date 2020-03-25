# Report 'cat vs mycat'
The function cat was test against the c program mycat, to check performance.
Two commands were used in order to achieve this:
$time cat <file>
$time ./mycat <file>

#### These were the results for cat:
- For a 20 kb .txt file:
    - real: 0m0.001s
    - user: 0m0.001s
    - sys: 0m0.000s
- For a 70 kb .txt file:
    - real: 0m0.004s
    - user: 0m0.002s
    - sys: 0m0.000s
- For a 145 kb .txt file:
    - real: 0m0.004s
    - user: 0m0.002s
    - sys: 0m0.000s
#### These were the results for mycat:
- For a 20 kb .txt file:
    - real: 0m0.001s
    - user: 0m0.001s
    - sys: 0m0.000s
- For a 70 kb .txt file:
    - real: 0m0.003s
    - user: 0m0.001s
    - sys: 0m0.000s
- For a 145 kb .txt file:
    - real: 0m0.003s
    - user: 0m0.001s
    - sys: 0m0.000s 
##### These were teste multiple times, and what was registered were the fastest in each trial. The mycat program, even though sometimes was slower, it was faster in average than the cat command.
