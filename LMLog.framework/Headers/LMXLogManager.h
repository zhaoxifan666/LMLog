//
//  LMXLogManager.h
//  LMLog
//
//  Created by 禹祥瑞 on 2021/8/3.
//

#import <Foundation/Foundation.h>


#ifndef _LMXLog_
#define _LMXLog_

#define kMaxLenght (2048)

typedef NS_ENUM(NSUInteger, LMXLogLevel)
{
    LMXLogLevelDebug = 1,
    LMXLogLevelInfo,
    LMXLogLevelWarn,
    LMXLogLevelError,    
};

@interface LMXLogFileInfo : NSObject

/**
 设置log名字前缀，日志名字每次启动生成，默认值是空，格式是：LogNamePrefix_yyyyMMdd_x.xlog
 */
@property (nonatomic, strong) NSString *logNamePrefix;

/**
 写日志的目录路径，该路径下不要放其他文件，默认值是NSDocumentDirectory/log/，日志路径格式是logDirPath/LogNamePrefix_yyyyMMdd_x.xlog
 */
@property (nonatomic, strong) NSString *logDirPath;

/**
 日志文件最大值，默认是2M（2*1024*1024）
 */
@property (nonatomic, assign) NSUInteger maxFileByteSize;

/**
 日志加密公钥，生成方式见文档
 */
@property (nonatomic, strong) NSString *ECCEncryptPublicKey;

/**
 写入日志级别,默认error级别
 */
@property (nonatomic, assign) LMXLogLevel logLevel;

/**
 是否输出到控制台，默认不输出
 */
@property (nonatomic, assign) BOOL printAtConsole;

@end


@interface LMXLogManager : NSObject

+ (instancetype)sharedManager;

/**
 设置日志信息
 */
- (void)setupOnMainFunc:(LMXLogFileInfo*)info;        // main函数调用，初始化日志组件，在这之前的所有日志都会丢
- (void)closeOnTerminate;       // 结束时调用

// Manage Log File
- (void)saveLogToFileAsync:(BOOL)isAsync;  // 主动将内存中的日志写到文件，isAsync：是否异步执行


// Write Log
- (void)writeLogFile:(const char *)file
            function:(const char *)function
                line:(int)line
               level:(LMXLogLevel)level
                 tag:(NSString *)tag
              format:(NSString *)format, ... NS_FORMAT_FUNCTION(6,7);

- (void)writeLogFile:(const char *)file
            function:(const char *)function
                line:(int)line
               level:(LMXLogLevel)level
                 tag:(NSString *)tag
              format:(NSString *)format
                args:(va_list)args;


/**
 获取当前所有日志文件的路径，按照时间排序（最新在前）
 
 @return 日志文件路径列表
 */
- (NSArray<NSString *> *)allSortedLogFilePaths;

/**
 获取指定日期当天的所有日志文件路径，按照时间排序（最新在前）
 
 @param date 日期（只使用到天，不会精确到小时分秒）
 @return 当天日志文件路径列表
 */
- (NSArray<NSString *> *)allSortedLogFilePathsAtDate:(NSDate *)date;

/**
 获取指定日期之后所有日志文件路径，按照时间排序（最新在前）
 
 @param date 日期（只使用到天，不会精确到小时分秒）
 @return 指定日期之后所有日志文件路径列表（不包括指定日期）
 */
- (NSArray<NSString *> *)allSortedLogFilePathsAfterDate:(NSDate *)date;

/**
 删除date之前的日志文件，date当天的不删除

 */
- (void)deleteLogFilesTo:(NSDate*)date;

/**
 删除所有日志文件（慎用）
 
 @return 是否删除成功
 */
- (BOOL)deleteAllLogFiles;

@end

#endif

#pragma mark - Log Marcos
// Base
#ifndef LMXLogWithLevelAndTag
#define LMXLogWithLevelAndTag(lvl, tg, frmt, ...) [[LMXLogManager sharedManager] writeLogFile:__FILE__ function:__PRETTY_FUNCTION__ line:__LINE__ level:lvl tag:tg format:frmt, ##__VA_ARGS__];
#endif

// Debug
#ifndef LMXLogDebugWithTag
#define LMXLogDebugWithTag(tg, frmt, ...) LMXLogWithLevelAndTag(LMXLogLevelDebug, tg, frmt, ##__VA_ARGS__)
#endif
#ifndef LMXLogDebug
#define LMXLogDebug(frmt, ...) LMXLogDebugWithTag(nil, frmt, ##__VA_ARGS__)
#endif

// Info
#ifndef LMXLogInfoWithTag
#define LMXLogInfoWithTag(tg, frmt, ...) LMXLogWithLevelAndTag(LMXLogLevelInfo, tg, frmt, ##__VA_ARGS__)
#endif
#ifndef LMXLogInfo
#define LMXLogInfo(frmt, ...) LMXLogInfoWithTag(@"0", frmt, ##__VA_ARGS__)
#endif

// Warning
#ifndef LMXLogWarnWithTag
#define LMXLogWarnWithTag(tg, frmt, ...) LMXLogWithLevelAndTag(LMXLogLevelWarn, tg, frmt, ##__VA_ARGS__)
#endif
#ifndef LMXLogWarn
#define LMXLogWarn(frmt, ...) LMXLogWarnWithTag(nil, frmt, ##__VA_ARGS__)
#endif

// Error
#ifndef LMXLogErrorWithTag
#define LMXLogErrorWithTag(tg, frmt, ...) LMXLogWithLevelAndTag(LMXLogLevelError, tg, frmt, ##__VA_ARGS__)
#endif
#ifndef LMXLogError
#define LMXLogError(frmt, ...) LMXLogErrorWithTag(nil, frmt, ##__VA_ARGS__)
#endif

// Common
#ifndef LMXLog
#define LMXLog(frmt, ...) LMXLogInfoWithTag(@"LM", frmt, ##__VA_ARGS__)
#endif
//#define NSLog(frmt, ...) LMXLogInfoWithTag(@"NS", frmt, ##__VA_ARGS__)

