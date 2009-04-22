//
//  LuaState.m
//
//  Created by Philippe Hausler on 3/24/09.
//  Copyright 2009 Philippe Hasler. All rights reserved.
//

#import "LuaState.h"
#include "lualib.h"

@interface LuaState (Private)
- (void)_output:(NSString *)str;
- (void)_error:(NSString *)err;
@end

@implementation LuaState
@synthesize delegate;
@synthesize encoding;

#pragma mark Initialization
+ (id)ready
{
	LuaState *lua = [[[self alloc] init] autorelease];
	return lua;
}

+ (id)state:(lua_State *)s
{
	lua_pushlightuserdata(s, (void *)s);
    lua_gettable(s, LUA_REGISTRYINDEX);
	return (LuaState *)lua_touserdata(s, -1);
}

- (id)init
{
	self = [super init];
	
	invocations = [[NSMutableArray alloc] init];
	
	encoding = [NSString defaultCStringEncoding];
	
	L = lua_open();
	
	[self pushLightUserdata:L];
	[self pushLightUserdata:self];
	[self setTable:LUA_REGISTRYINDEX];

	return self;
}
- (void)loadTable
{
	luaopen_table(L);
}

- (void)loadString
{
	luaopen_string(L);
}

- (void)loadMath
{
	luaopen_math(L);
}

- (void)loadIO
{
	luaopen_io(L);
}

- (void)loadPackage
{
	luaopen_package(L);
}

- (void)loadDebug
{
	luaopen_debug(L);
}
#pragma mark Cleanup
- (void)close
{
	lua_close(L);
}
- (void)dealloc
{
	[invocations release];
	if(L)
	{
		[self close];
	}
	[super dealloc];
}

#pragma mark Property Access
- (lua_State *)state
{
	return L;
}

#pragma mark Script Execution
- (void)runScript:(NSString *)script
{
	if([self loadBuffer:script] || [self pcall:0 results:0 error:0])
	{
		[self _error:[self toString:-1]];
	}
}

#pragma mark Buffer Loading
- (int)loadBuffer:(NSString *)buffer
{
	return [self loadBuffer:buffer named:NULL];
	
}

- (int)loadBuffer:(NSString *)buffer named:(NSString *)name
{
	if(name == NULL)
		return luaL_loadbuffer(L, [buffer cStringUsingEncoding:encoding], [buffer length], nil);
	else
		return luaL_loadbuffer(L, [buffer cStringUsingEncoding:encoding], [buffer length], [name cStringUsingEncoding:encoding]);
}

#pragma mark Calling
- (int)pcall:(int)arguments results:(int)results error:(int)errFunctions
{
	return lua_pcall(L, arguments, results, errFunctions);
}

- (void)call:(int)arguments results:(int)results
{
	lua_call(L, arguments, results);
}

#pragma mark Libraries
- (void)openlib:(NSString *)name withFunctions:(const luaL_Reg *)registration index:(int)index
{
	luaL_openlib(L, [name cStringUsingEncoding:encoding], registration, index); 
}

#pragma mark Errors
- (void)typeError:(int)index types:(NSString *)expectedTypes
{
	luaL_typerror(L, index, [expectedTypes cStringUsingEncoding:encoding]);
}
@end

@implementation LuaState (Private)
- (void)_output:(NSString *)str
{
	if(delegate)
	{
		if([delegate respondsToSelector:@selector(state:output:)])
		{
			[delegate state:self output:str];
		}
	}
}
- (void)_error:(NSString *)err
{
	if(delegate)
	{
		if([delegate respondsToSelector:@selector(state:error:)])
		{
			[delegate state:self error:err];
		}
	}
}
@end

@implementation LuaState (Strings)
- (NSString *)toString:(int)index
{
	return [NSString stringWithCString:lua_tostring(L, index) encoding:encoding];
}
- (NSString *)checkString:(int)index
{
	return [NSString stringWithCString:luaL_checkstring(L, index) encoding:encoding];
}
- (BOOL)isString:(int)index
{
	return lua_isstring(L, index);
}
- (void)pushString:(NSString *)string
{
	lua_pushstring(L, [string cStringUsingEncoding:encoding]);
}
@end

@implementation LuaState (Numbers)
#pragma mark NSNumber
- (NSNumber *)toNumber:(int)index
{
	return [NSNumber numberWithDouble:lua_tonumber(L, index)];
}
- (NSNumber *)checkNumber:(int)index
{
	return [NSNumber numberWithDouble: luaL_checknumber(L, index)];
}
- (BOOL)isNumber:(int)index
{
	return lua_isnumber(L, index);
}
- (void)pushNumber:(NSNumber *)number
{
	[self pushDouble:[number doubleValue]];
}

#pragma mark NSInteger
- (NSInteger)toInteger:(int)index
{
	return lua_tointeger(L, index);
}

- (NSInteger)checkInteger:(int)index
{
	return luaL_checkint(L, index);
}

- (BOOL)isInteger:(int)index
{
	return [self isNumber:index];
}

- (void)pushInteger:(NSInteger)number
{
	lua_pushinteger(L, number);
}

#pragma mark NSUInteger
- (NSUInteger)toUnsignedInteger:(int)index
{
	return [[self toNumber:index] integerValue];
}

- (NSUInteger)checkUnsignedInteger:(int)index
{
	return [[self checkNumber:index] integerValue];
}

- (BOOL)isUnsignedInteger:(int)index
{
	return lua_isnumber(L, index);
}

- (void)pushUnsignedInteger:(NSUInteger)number
{
	[self pushNumber:[NSNumber numberWithInteger:number]];
}

#pragma mark Float
- (float)toFloat:(int)index
{
	return [[self toNumber:index] floatValue];
}

- (float)checkFloat:(int)index
{
	return [[self checkNumber:index] floatValue];
}

- (BOOL)isFloat:(int)index
{
	return lua_isnumber(L, index);
}

- (void)pushFloat:(float)number
{
	[self pushNumber:[NSNumber numberWithFloat:number]];
}

#pragma mark Double
- (double)toDouble:(int)index
{
	return lua_tonumber(L, index);
}

- (double)checkDouble:(int)index
{
	return luaL_checknumber(L, index);
}

- (BOOL)isDouble:(int)index
{
	return lua_isnumber(L, index);
}

- (void)pushDouble:(double)number
{
	return lua_pushnumber(L, number);
}

#pragma mark Boolean
- (BOOL)toBoolean:(int)index
{
	return lua_toboolean(L, index);
}

- (BOOL)checkBoolean:(int)index
{
	[self checkType:LuaStateBooleanType index:index];
	return [self toBoolean:index];
}

- (BOOL)isBoolean:(int)index
{
	return lua_isboolean(L, index);
}

- (void)pushBoolean:(BOOL)tf
{
	lua_pushboolean(L, tf);
}

@end

@implementation LuaState (Stack)
#pragma mark Types
- (LuaStateType)type:(int)index
{
	return lua_type(L, index);
}

- (NSString *)typeName:(int)index
{
	return [LuaState typeName:[self type:index]];
}

+ (NSString *)typeName:(LuaStateType)type
{
	switch (type) {
		case LuaStateNoneType:
			return @"none";
			break;
		case LuaStateNullType:
			return @"nil";
			break;
		case LuaStateBooleanType:
			return @"boolean";
			break;
		case LuaStateLightUserdataType:
		case LuaStateUserdataType:
			return @"userdata";
			break;
		case LuaStateNumberType:
			return @"number";
			break;
		case LuaStateStringType:
			return @"string";
			break;
		case LuaStateTableType:
			return @"table";
			break;
		case LuaStateFunctionType:
			return @"function";
			break;
		case LuaStateThreadType:
			return @"thread";
			break;
		case LuaStateProtoType:
			return @"proto";
			break;
		case LuaStateUpvalType:
			return @"upval";
			break;
		default:
			return @"unknown";
			break;
	}
}

- (void)checkType:(LuaStateType)type index:(int)index
{
	luaL_checktype(L, index, type);
}

- (void)pushValue:(int)index
{
	lua_pushvalue(L, index);
}

- (void)pushLiteral:(const char *)str
{
	lua_pushlstring(L, str, (sizeof(str)/sizeof(char))-1);
}

- (void)setTop:(int)index
{
	lua_settop(L, index);
}

- (int)getTop
{
	return lua_gettop(L);
}

- (void)pop:(int)index
{
	lua_pop(L, index);
}

- (void)insert:(int)index
{
	lua_insert(L, index);
}

- (void)remove:(int)index
{
	lua_remove(L, index);
}

- (int)next:(int)index
{
	return lua_next(L, index);
}

- (void)pushNil
{
	lua_pushnil(L);
}

- (void)getGlobal:(NSString *)name
{
	lua_getglobal(L, [name cStringUsingEncoding:encoding]);
}

- (void)setGlobal:(NSString *)name
{
	lua_setglobal(L, [name cStringUsingEncoding:encoding]);
}

- (NSString *)description:(int)index
{
	NSString *description;
	[self getGlobal:@"tostring"];
	[self pushValue:-1];
	[self pushValue:index];
	[self call:1 results:1];
	description = [self toString:-1];
	[self pop:1];
	return description;
}
@end

@implementation LuaState (Tables)
- (void)newTable
{
	lua_newtable(L);
}

- (BOOL)isTable:(int)index
{
	return lua_istable(L, index);
}

- (void)getTable:(int)index
{
	lua_gettable(L, index);
}

- (void)setTable:(int)index
{
	lua_settable(L, index);
}

- (void)rawset:(int)index
{
	lua_rawset(L, index);
}

- (void)rawget:(int)index
{
	lua_rawget(L, index);
}

- (void)getField:(int)index named:(NSString *)name
{
	lua_getfield(L, index, [name cStringUsingEncoding:encoding]);
}

- (void)setField:(int)index named:(NSString *)name
{
	lua_setfield(L, index, [name cStringUsingEncoding:encoding]);
}

- (NSString *)findTableNamed:(NSString *)fname index:(int)index withSizeHint:(int)szhint
{
	const char *t = luaL_findtable(L, index, [fname cStringUsingEncoding:encoding], szhint);
	if(t == NULL)
		return NULL;
	else
		return [NSString stringWithFormat:@"%s", t];
}
@end

@implementation LuaState (Metatables)
- (void)newMetatable:(NSString *)name
{
	luaL_newmetatable(L, [name cStringUsingEncoding:encoding]);
}

- (int)getMetatable:(int)index
{
	return lua_getmetatable(L, index);
}

- (int)setMetatable:(int)index
{
	return lua_setmetatable(L, index);
}

- (void)getMetatableField:(NSString *)fieldName
{
	luaL_getmetatable(L, [fieldName cStringUsingEncoding:encoding]);
}
@end

@implementation LuaState (Functions)
- (lua_CFunction)toCFunction:(int)index
{
	return lua_tocfunction(L, index);
}

- (lua_CFunction)checkCFunction:(int)index
{
	[self checkType:LuaStateFunctionType index:index];
	return lua_tocfunction(L, index);
}

- (BOOL)isFunction:(int)index
{
	return lua_iscfunction(L, index);
}

- (BOOL)isLuaFunction:(int)index
{
	return ([self type:index] == LuaStateFunctionType && ![self isCFunction:index]);
}

- (BOOL)isCFunction:(int)index
{
	return lua_iscfunction(L, index);
}

- (void)pushFunction:(lua_CFunction)func
{
	lua_pushcfunction(L, func);
}

- (void)pushCClosure:(lua_CFunction)func index:(int)index
{
	lua_pushcclosure(L, func, index);
}

int lobjc_inv(lua_State *L)
{
	int ret = 0;
	NSInvocation *inv = (NSInvocation *)lua_touserdata(L, lua_upvalueindex(1));
	LuaState *state = [LuaState state:L];
	[inv setArgument:&state atIndex:3];
	[inv invoke];
	[inv getReturnValue:&ret];
	return ret;
}

- (void)pushSelector:(SEL)selector forObject:(id)obj
{
	NSMethodSignature * sig = [[obj class] instanceMethodSignatureForSelector:selector];
	NSInvocation * inv = [NSInvocation invocationWithMethodSignature:sig];
	if([obj respondsToSelector:selector])
	{
		if([sig numberOfArguments] == 3 && [sig methodReturnLength] == 4)
		{
			if(strcmp([sig methodReturnType], "i") == 0 &&
			   strcmp([sig getArgumentTypeAtIndex:0], "@") == 0 &&
			   strcmp([sig getArgumentTypeAtIndex:1], ":") == 0 &&
			   strcmp([sig getArgumentTypeAtIndex:2], "@") == 0)
			{
				[inv setTarget:obj];
				[inv setSelector:selector];
				[invocations addObject:inv];
				[self pushLightUserdata:inv];
				[self pushCClosure:lobjc_inv index:1];
			}
			else
			{
				NSLog(@"Fatal error! Method signature for selector must be in the format @selector(function:) and have a prototype like -(int)function:(LuaState *)L");
			}
		}
		else
		{
			NSLog(@"Fatal error! Method signature for selector must be in the format @selector(function:) and have a prototype like -(int)function:(LuaState *)L but found %@", NSStringFromSelector(selector));
		}
	}
	else
	{
		NSLog(@"Fatal error! %@ does not respond to the selector %@, please try again...", [obj description], NSStringFromSelector(selector));
	}
}
@end

@implementation LuaState (Userdata)
- (void *)newUserdata:(size_t)size
{
	return lua_newuserdata (L, size);
}

- (void *)toUserdata:(int)index
{
	return lua_touserdata(L, index);
}
- (void *)checkUserdata:(int)index
{
	if(![self type:index] == LuaStateUserdataType || ![self type:index] == LuaStateLightUserdataType)
	{
		[self typeError:index types:[LuaState typeName:LuaStateUserdataType]];
	}
	return [self toUserdata:index];
}
- (BOOL)isUserdata:(int)index
{
	return lua_isuserdata(L, index);
}

- (BOOL)isLightUserdata:(int)index
{
	return lua_islightuserdata(L, index);
}

- (void)pushLightUserdata:(void *)userdata
{
	lua_pushlightuserdata(L, userdata);
}
@end