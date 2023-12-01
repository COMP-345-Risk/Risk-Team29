# Team

- Eyal Azimov 
- Liora Mezirovsky

# How to run our project?

### Code Runner Mac
- Go to CodeRunner Extension settings
- Look for Executor Map and open "Edit in settings.json" in blue
- replace the "cpp" code with the one below
- Install the extension `Code Runner`
- Add the following line in `execution map`'s setting.json file:
```bash
        "cpp": "cd $dir && g++ -std=c++14 $fileName `find . \\( -iname '*.cpp' -not -name '$fileName' \\)` -o $fileNameWithoutExt && $dir$fileNameWithoutExt",

```
your file should look like this:
```bash
  "code-runner.executorMap": {

        "javascript": "node",
        "java": "cd $dir && javac $fileName && java $fileNameWithoutExt",
        "c": "cd $dir && gcc $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
        "zig": "zig run",
        "cpp": "cd $dir && g++ -std=c++14 $fileName `find . \\( -iname '*.cpp' -not -name '$fileName' \\)` -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
....
}
```

# Console input for running tournament tests

tournament -M Africa Europe solarSystem -P  Aggressive Benevolent Neutral Cheater  -G 2 -D 10

