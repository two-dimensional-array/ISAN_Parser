import subprocess

testData = [
  {
    "data"    : "[1, 2, \"three\", [4, [5], [\"six\"]], [[[\"seven\"]]]]",
    "command" : "0t",
    "result"  : "int"
  },
  {
    "data"    : "[1, 2, \"three\", [4, [5], [\"six\"]], [[[\"seven\"]]]]",
    "command" : "2v",
    "result"  : "three"
  },
  {
    "data"    : "[1, 2, \"three\", [4, [5], [\"six\"]], [[[\"seven\"]]]]",
    "command" : "3.1v",
    "result"  : "array"
  },
  {
    "data"    : "[1, 2, \"three\", [4, [5], [\"six\"]], [[[\"seven\"]]]]",
    "command" : "4.0.0.0v",
    "result"  : "seven"
  }
]

for case in testData:
  proc = subprocess.Popen(f'echo \'{case["data"]}\' | ./parse_isan {case["command"]}', shell=True, stdout=subprocess.PIPE)
  out = proc.stdout.readline().decode()
  print("Test Passed" if out == f'{case["result"]}\n' else "Test Failed")
  print(f'Input data    : {case["data"]}')
  print(f'Input command : {case["command"]}')
  print(f'Expected      : {case["result"]}')
  print(f'Actual        : {out}')
