# Contributing

## Development environment

### Windows

1. **Prerequisites**
   - Install the **PS3 4.75 SDK Offline Installer**.
   - Install **MSYS2**.
   - Install **Visual Studio Code**.

2. **Setup MSYS2**
   - Open an **MSYS2** shell in the project folder.
   - Update your **MSYS2** installation and install necessary build tools.
   - Integrate the **MSYS2** shell into **vscode**.

3. **Configure VS Code**
   - Install recommended extensions.
   - In `CenturyPackage.code-workspace`, set your **console IP** and **game version**.
   - Make sure an FTP server is running on your console (e.g., via **webMAN**).

4. **Build & Deploy**
   - **With MSYS2**

     ```bash
     make release
     ```

     This outputs your module into `bin/release/`.
   - **With VS Code**
     - Open the project folder.
     - Press <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>B</kbd> to trigger the build or deployment tasks.
     - Deployment tasks will create or replace the compiled module and any game scripts to your console.

5. **Debugging & Output**  
   - Any output from the custom `printf` implementation can be viewed in the **Target Manager** under the **Console Output** menu.
