<!--
 ~ SPDX-FileCopyrightText: Copyright DB Netz AG
 ~ SPDX-License-Identifier: CC0-1.0
 -->

# DBS Onboard C++ Map API Contributor Guide

## Contributing to DBS Onboard C++ Map API

Thanks for your interest in our project. Contributions are always welcome!

We are committed to fostering a welcoming, respectful, and harassment-free
environment. Be kind!

If you have questions, ideas or want to report a bug, feel free to [open an
issue](https://github.com/DSD-DBS/dbs-onboard-map-api/issues). Or go ahead and [open a pull request](https://github.com/DSD-DBS/dbs-onboard-map-api/pulls) to contribute code. In order
to reduce the burden on our maintainers, please make sure that your code
follows our style guidelines outlined below.

## Development

### Code style
This project follows [custom code style](https://github.com/DSD-DBS/dbs-onboard-map-api/blob/main/.clang-format). Code style is checked as pre-commit hook.

#### Pre-Commit

We use [pre-commit](https://pre-commit.com/) to enforce usage of tools such as clang-format on commit. This ensures that tools such as clang-format are run on a commit, and thus keeps our repository consistent and makes review easier.

Install all the hooks with:
```bash
pre-commit install
```

Pre-commit will be then automatically run on changed files in a commit. Additionally, it can also be run manually against all files with:
```bash
pre-commit run --all-files
```

#### Component documentation

The Map C++ API uses [Doxygen](http://www.doxygen.nl/) to generate its documentation from the code.
Make sure to properly document each header file that is exposed by the component with the use of Doxygen format.