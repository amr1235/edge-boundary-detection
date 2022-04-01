# Contributing

All changes should take place in a separate branch submitted to the master as a _pull request_. Ideally, a pull request should be small not 500 lines of code spread out on 10 separate files :)

## Steps:

1. Create a new Branch.

```sh
$ git checkout -b <branch>
```

2. Add and commit your changes.
```sh
$ git add .
$ git commit -m "commit message"
```
3. After commiting your changes, merge to make sure you have the latest changes from the master.

```bash
$ git checkout master
$ git pull origin master
$ git checkout <branch>
$ git merge master
```

4. Push your branch to the origin and visit github to create a pull request.
```sh
$ git push origin <branch>
```

5. After your changes had been successfully merged into the master branch, your branch will be deleted at the origin but not locally, so make sure you clean up your local.

```sh
> git branch -D <branch>
```

Useful commit messages that describe what you have been working on are greatly appreciated, [here](https://www.conventionalcommits.org/en/v1.0.0/#summary) is a really great guide for writing a helpful commit message.