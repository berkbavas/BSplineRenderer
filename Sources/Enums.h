#pragma once

enum class Mode {
    Select = 0, //
    Add
};

enum class Action {
    Select = 0, //
    AddKnot,
    UpdateMode,
    UpdateKnotPointPositionFromScreen,
    UpdateKnotPointPositionFromGui,
    UpdateRenderPaths,
    UpdateRenderPipes,
    UpdateSelectedCurvePipeRadius,
    UpdateSelectedCurvePipeSectorCount,
    UpdateGlobalPipeRadius,
    UpdateGlobalPipeSectorCount,
    RemoveSelectedCurve,
    RemoveSelectedKnotPoint,
    ClearScene,
    ShowImportWindow,
    ShowExportWindow,
    Import,
    Export

};

