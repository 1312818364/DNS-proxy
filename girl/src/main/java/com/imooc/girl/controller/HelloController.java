package com.imooc.girl.controller;

import com.imooc.girl.properties.GirlProperties;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

//结合使用等同于 RestController
//@Controller
//@ResponseBody
@RestController
//等同于增加content_path
//@RequestMapping("girl")
public class HelloController {
     @Autowired
     GirlProperties girlProperties;
//    @Value("${cupSize}")
//    private String cupSize;
//
//    @Value("${age}")
//    private Integer age;
//
//    @Value("${content}")
//    private String content;

    @RequestMapping(value = "/say", method=RequestMethod.GET)
    public String say(@RequestParam(value="id", required = false, defaultValue = "0") Integer id) {

        return "id: "+ id;
        // return girlProperties.getCupSize();
        //        return "index";
    }
}
