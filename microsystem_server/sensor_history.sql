
SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `sensor_history`
-- ----------------------------
DROP TABLE IF EXISTS `sensor_history`;
CREATE TABLE `sensor_history` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `device_id` char(255) NOT NULL COMMENT '设备编号',
  `temp` double(11,1) NOT NULL COMMENT '温度传感器结果',
  `hum` double(11,1) NOT NULL COMMENT '湿度传感器结果',
  `light` double(11,1) NOT NULL COMMENT '光照传感器结果',
  `time` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sensor_history
-- ----------------------------
INSERT INTO `sensor_history` VALUES ('1', '12CD', '18.0', '58.0', '1802.0', '2016-09-03 21:14:51');
INSERT INTO `sensor_history` VALUES ('2', 'CD12', '28.0', '68.0', '2300.0', '2016-09-03 21:16:49');
INSERT INTO `sensor_history` VALUES ('3', 'EE12', '32.0', '70.0', '1900.0', '2016-09-03 21:17:07');
