/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50714
Source Host           : localhost:3306
Source Database       : monitor

Target Server Type    : MYSQL
Target Server Version : 50714
File Encoding         : 65001

Date: 2016-09-25 15:01:08
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `sensor_history`
-- ----------------------------
DROP TABLE IF EXISTS `sensor_history`;
CREATE TABLE `sensor_history` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `device_id` char(255) NOT NULL COMMENT '设备MAC地址',
  `hum` double(11,1) NOT NULL COMMENT '湿度结果',
  `temp` double(11,1) NOT NULL COMMENT '温度结果',
  `light` double(11,1) NOT NULL COMMENT '光照结果',
  `time` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sensor_history
-- ----------------------------
INSERT INTO `sensor_history` VALUES ('1', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:14:51');
INSERT INTO `sensor_history` VALUES ('2', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:15:56');
INSERT INTO `sensor_history` VALUES ('3', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:16:19');
INSERT INTO `sensor_history` VALUES ('4', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:16:21');
INSERT INTO `sensor_history` VALUES ('5', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:16:22');
INSERT INTO `sensor_history` VALUES ('6', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:16:23');
INSERT INTO `sensor_history` VALUES ('7', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:16:24');
INSERT INTO `sensor_history` VALUES ('8', '12CD', '90.0', '24.0', '182.0', '2016-09-03 21:16:25');
INSERT INTO `sensor_history` VALUES ('9', 'CD12', '90.0', '24.0', '182.0', '2016-09-03 21:16:49');
INSERT INTO `sensor_history` VALUES ('10', 'CD12', '90.0', '24.0', '182.0', '2016-09-03 21:16:50');
INSERT INTO `sensor_history` VALUES ('11', 'CD12', '90.0', '24.0', '182.0', '2016-09-03 21:16:51');
INSERT INTO `sensor_history` VALUES ('12', 'CD12', '90.0', '24.0', '182.0', '2016-09-03 21:16:52');
INSERT INTO `sensor_history` VALUES ('13', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:17:07');
INSERT INTO `sensor_history` VALUES ('14', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:17:08');
INSERT INTO `sensor_history` VALUES ('15', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:17:09');
INSERT INTO `sensor_history` VALUES ('16', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:17:10');
INSERT INTO `sensor_history` VALUES ('17', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:17:11');
INSERT INTO `sensor_history` VALUES ('18', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:17:12');
INSERT INTO `sensor_history` VALUES ('19', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:38:26');
INSERT INTO `sensor_history` VALUES ('20', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 21:53:50');
INSERT INTO `sensor_history` VALUES ('21', 'EE12', '90.0', '24.0', '182.0', '2016-09-03 22:04:30');
INSERT INTO `sensor_history` VALUES ('22', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:36');
INSERT INTO `sensor_history` VALUES ('23', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:37');
INSERT INTO `sensor_history` VALUES ('24', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:39');
INSERT INTO `sensor_history` VALUES ('25', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:40');
INSERT INTO `sensor_history` VALUES ('26', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:41');
INSERT INTO `sensor_history` VALUES ('27', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:41');
INSERT INTO `sensor_history` VALUES ('28', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:42');
INSERT INTO `sensor_history` VALUES ('29', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:43');
INSERT INTO `sensor_history` VALUES ('30', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:17:44');
INSERT INTO `sensor_history` VALUES ('31', '12CD', '90.0', '1234.0', '1720.0', '2016-09-05 09:23:47');
