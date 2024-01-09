# SanaEngine

A visual localization project based on opengl

## Dependencies

dependencies 文件中的所有依赖库已经用MSVC编译好，目前只支持windows，在linux使用需要重新编译。lib和dll在gitignore里，重新拉取项目需要复制过来
### opencv环境
Opencv版本是Opencv4.7，用cuda11.1编译，运行需要cuda runtime 11.1以上，建议使用11.3，需要安装对应版本cudnn

### 字体依赖
用了SourceHanSansSC-Regular-2.otf字体，不在git里，重新拉取项目要复制

## Build
- 构建使用CMAKE，编译器请使用CL编译器。 从Developer powershell for vs打开vscode，点击下方编译和运行按钮即可
- 由于Opencv只编译了release版本，不支持Debug编译选项，可以使用RelWithDebInfo调试。
- build target请选择Install


## doc
数据集生成代码在ECS->COMPONENT->DatasetGenComponent里