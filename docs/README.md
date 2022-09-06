<!--
  ~ Copyright 2022 NWChemEx-Project
  ~
  ~ Licensed under the Apache License, Version 2.0 (the "License");
  ~ you may not use this file except in compliance with the License.
  ~ You may obtain a copy of the License at
  ~
  ~ http://www.apache.org/licenses/LICENSE-2.0
  ~
  ~ Unless required by applicable law or agreed to in writing, software
  ~ distributed under the License is distributed on an "AS IS" BASIS,
  ~ WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ~ See the License for the specific language governing permissions and
  ~ limitations under the License.
-->

Building the documentation
==========================

Building is done by running:

```
make html
```

in the `Utilities/docs` directory. If this runs successfully you will get two
additional directories: `build` and `doxyoutput`. The actual documentation can
be viewed by opening `docs/build/html/index.html` in a web browser.

If the build did not run successfully check that you have installed the
following Python packages (all available in `pip`):

- sphinx_rtd_theme (The Read-The-Docs theme for sphinx)
- sphinx (The thing that makes the documenation)
- breathe (Dependency of exhale, may get installed by it)
- exhale (The thing that turns Doxygen output into ReST)

You also will need Doxygen.

