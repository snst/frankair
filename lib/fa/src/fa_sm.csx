#!/usr/bin/env dotnet-script
// This is a c# script file

#r "nuget: StateSmith, 0.7.16-alpha" // this line specifies which version of StateSmith to use and download from c# nuget web service.

// spell-checker: ignore drawio

//using StateSmith.Input.Expansions;
//using StateSmith.Output.UserConfig;
//using StateSmith.Runner;

using StateSmith.Input.Expansions;
using StateSmith.Output;
//using StateSmith.Output.Gil.C99;
using StateSmith.Output.UserConfig;
using StateSmith.Runner;
using StateSmith.SmGraph;

SmRunner runner = new(diagramPath: "fa_sm.drawio.svg", new MyGlueLogic());
//var customizer = runner.GetExperimentalAccess().DiServiceProvider.GetInstanceOf<GilToC99Customizer>();
//customizer.CFileNameBuilder = (StateMachine sm) => $"{sm.Name}.cpp";
//customizer.EnumDeclarationBuilder = (string enumName) => $"typedef enum __attribute__((packed)) {enumName}";

runner.Run();

// ignore C# guidelines for script stuff below
#pragma warning disable IDE1006, CA1050 

/// <summary>
/// This class gives StateSmith the info it needs to generate working C code.
/// It adds user code to the generated .c/.h files, declares user variables,
/// and provides diagram code expansions. This class can have any name.
/// </summary>
public class MyGlueLogic :  IRenderConfigC //, IRenderConfig
{
    // You can define RenderConfig options in the diagram now if you like.
    // See https://github.com/StateSmith/StateSmith/issues/23
    string IRenderConfigC.HFileIncludes => @"
        // user IRenderConfigC.HFileIncludes: whatever you want to put in here.
    ";

    string IRenderConfigC.CFileIncludes => @"
        // user IRenderConfigC.CFileIncludes: whatever you want to put in here.
    ";
/*
    string IRenderConfig.VariableDeclarations => @"
        // user IRenderConfig.VariableDeclarations: whatever you want to put in here.
        ccc
    ";
*/
    /// <summary>
    /// This nested class creates expansions because it is inside a class that implements `IRenderConfigC`.
    /// This class can have any name.
    /// </summary>
    public class MyExpansions : UserExpansionScriptBase
    {
        //string fire_ev(string id, string ms) => $"self->vars.eventSM.send(fa_sm_gen_EventId_{id}, {ms})";
        string fire_ev(string id, string ms) => $"self->vars.eventSM.send(fa_sm_gen_EventId_{id}, ({ms} * 1000U))";
    }
}
