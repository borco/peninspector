# Packaging

## macOS

### create-dmg

[create-dmg](https://github.com/create-dmg/create-dmg) is required to build the installer .dmg file. You can install it with `brew` or from sources.

```sh
brew install create-dmg
```

### Configure QtCreator packaging build configuration

* go to *Projects* and clone the *Release* build and name it *Packaging*, for example
* select the *Build Steps* settings and click the *Details*
* select the **PenInspector_mac_build_dmg** target
* click the *Build* button or type the build shortcut

## Windows

### Configure QtCreator packaging build configuration

* go to *Projects* and clone the *Release* build and name it *Packaging*, for example
* select the *Build Steps* settings and click the *Details*
* select the **PenInspector_windeployqt** target
* click the *Build* button or type the build shortcut
