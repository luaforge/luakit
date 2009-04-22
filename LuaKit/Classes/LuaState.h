//
//  LuaState.h
//
//  Created by Philippe Hausler on 3/24/09.
//  Copyright 2009 Philippe Hausler. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <LuaKit/lua.h>
#include <LuaKit/lobject.h>
#include <LuaKit/lauxlib.h>

typedef enum _LuaStateType {
	LuaStateNoneType =				LUA_TNONE,
	LuaStateNullType =				LUA_TNIL,
	LuaStateBooleanType =			LUA_TBOOLEAN,
	LuaStateLightUserdataType =		LUA_TLIGHTUSERDATA,
	LuaStateNumberType =			LUA_TNUMBER,
	LuaStateStringType =			LUA_TSTRING,
	LuaStateTableType =				LUA_TTABLE,
	LuaStateFunctionType =			LUA_TFUNCTION,
	LuaStateUserdataType =			LUA_TUSERDATA,
	LuaStateThreadType =			LUA_TTHREAD,
	LuaStateProtoType =				LUA_TPROTO,
	LuaStateUpvalType =				LUA_TUPVAL
} LuaStateType;

@class LuaState;
@protocol LuaStateDelegate <NSObject>
@required
- (void)state:(LuaState *)L error:(NSString *)err;
- (void)state:(LuaState *)L output:(NSString *)output;
@end


@interface LuaState : NSObject {
	lua_State *L;
	id<LuaStateDelegate> delegate;
	NSStringEncoding encoding;
	NSMutableArray *invocations;
}
@property (nonatomic, assign) id<LuaStateDelegate> delegate;
@property (nonatomic, readonly) lua_State *state;
@property (nonatomic, assign) NSStringEncoding encoding;
+ (id)ready;
+ (id)state:(lua_State *)s;

- (void)loadTable;
- (void)loadString;
- (void)loadMath;
- (void)loadIO;
- (void)loadPackage;
- (void)loadDebug;

- (void)close;

#pragma mark Script Execution
- (void)runScript:(NSString *)script;

#pragma mark Buffer Loading
- (int)loadBuffer:(NSString *)buffer;
- (int)loadBuffer:(NSString *)buffer named:(NSString *)name;

#pragma mark Calling
- (int)pcall:(int)arguments results:(int)results error:(int)errFunctions;
- (void)call:(int)arguments results:(int)results;

#pragma mark Libraries
- (void)openlib:(NSString *)name withFunctions:(const luaL_Reg *)registration index:(int)index;

#pragma mark Errors
- (void)typeError:(int)index types:(NSString *)expectedTypes;
@end

@interface LuaState (Strings)
- (NSString *)toString:(int)index;
- (NSString *)checkString:(int)index;
- (BOOL)isString:(int)index;
- (void)pushString:(NSString *)string;
@end

@interface LuaState (Numbers)
- (NSNumber *)toNumber:(int)index;
- (NSNumber *)checkNumber:(int)index;
- (BOOL)isNumber:(int)index;
- (void)pushNumber:(NSNumber *)number;

- (NSInteger)toInteger:(int)index;
- (NSInteger)checkInteger:(int)index;
- (BOOL)isInteger:(int)index;
- (void)pushInteger:(NSInteger)number;

- (NSUInteger)toUnsignedInteger:(int)index;
- (NSUInteger)checkUnsignedInteger:(int)index;
- (BOOL)isUnsignedInteger:(int)index;
- (void)pushUnsignedInteger:(NSUInteger)number;

- (float)toFloat:(int)index;
- (float)checkFloat:(int)index;
- (BOOL)isFloat:(int)index;
- (void)pushFloat:(float)number;

- (double)toDouble:(int)index;
- (double)checkDouble:(int)index;
- (BOOL)isDouble:(int)index;
- (void)pushDouble:(double)number;

- (BOOL)toBoolean:(int)index;
- (BOOL)checkBoolean:(int)index;
- (BOOL)isBoolean:(int)index;
- (void)pushBoolean:(BOOL)tf;
@end

@interface LuaState (Stack)
- (LuaStateType)type:(int)index;
- (NSString *)typeName:(int)index;
+ (NSString *)typeName:(LuaStateType)type;
- (void)checkType:(LuaStateType)type index:(int)index;

- (void)pushValue:(int)index;
- (void)pushLiteral:(const char *)str;

- (void)setTop:(int)index;
- (int)getTop;

- (void)pop:(int)index;
- (void)insert:(int)index;
- (void)remove:(int)index;

- (int)next:(int)index;

- (void)pushNil;

- (void)getGlobal:(NSString *)name;
- (void)setGlobal:(NSString *)name;

- (NSString *)description:(int)index;
@end

@interface LuaState (Tables)
- (void)newTable;

- (BOOL)isTable:(int)index;

- (void)getTable:(int)index;
- (void)setTable:(int)index;

- (void)rawset:(int)index;
- (void)rawget:(int)index;

- (void)getField:(int)index named:(NSString *)name;
- (void)setField:(int)index named:(NSString *)name;

- (NSString *)findTableNamed:(NSString *)fname index:(int)index withSizeHint:(int)szhint;
@end

@interface LuaState (Metatables)
- (void)newMetatable:(NSString *)name;

- (int)getMetatable:(int)index;
- (int)setMetatable:(int)index;

- (void)getMetatableField:(NSString *)fieldName;
@end

@interface LuaState (Functions)
- (lua_CFunction)toCFunction:(int)index;
- (lua_CFunction)checkCFunction:(int)index;
- (BOOL)isFunction:(int)index;
- (BOOL)isLuaFunction:(int)index;
- (BOOL)isCFunction:(int)index;

- (void)pushFunction:(lua_CFunction)func;
- (void)pushCClosure:(lua_CFunction)func index:(int)index;
- (void)pushSelector:(SEL)selector forObject:(id)obj;
@end

@interface LuaState (Userdata)
- (void *)newUserdata:(size_t)size;

- (void *)toUserdata:(int)index;
- (void *)checkUserdata:(int)index;
- (BOOL)isUserdata:(int)index;

- (BOOL)isLightUserdata:(int)index;

- (void)pushLightUserdata:(void *)userdata;
@end
