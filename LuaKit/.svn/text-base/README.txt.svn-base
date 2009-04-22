LuaKit.framework

About
LuaKit.framework is a full implementation of Lua 5.1.4 (with some alterations) with an Objective-C 
abstraction in a framework suitable for embedding in Mac OS X Cocoa applications. This includes 
Foundation native conversions and delegations to and from the state.

Significant innovations in this framework include but are not limited to Objective-C selector 
invocations being passable as closures to the state, string encoding support for UTF8, UTF16, JIS etc.

Usage
To add LuaKit to a project right click on the project and add an "Existing Framework..." then under 
the targets section right click the target that will use the framework and make a "New Build Phase" as
a "New Copy Files Phase..." with the destination set to Frameworks.

In any source file that needs to reference LuaKit objects just include <LuaKit/LuaState.h> and that file
will then have access to the framework objects as any other framework.