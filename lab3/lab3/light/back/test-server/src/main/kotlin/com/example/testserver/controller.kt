package com.example.testserver

import com.example.testserver.Comment
import org.springframework.web.bind.annotation.*
import java.util.*

@CrossOrigin
@RestController
@RequestMapping("/api")
class CommentController {
    private val list: MutableList<Comment> = LinkedList<Comment>()

    @PostMapping("/addComment") //addComments
    fun addComment(@RequestBody comment: Comment): String {
        if (comment.getAuthor()?.length!! < 1 || comment.getAuthor()!!.length > 100) return "400 Bad Request" //"username - required ﬁeld, maximum ﬁeld length 100 characters";
        if (comment.getContent()!!.length < 1 || comment.getContent()!!.length > 1000) return "400 Bad Request" //"comment - required ﬁeld, the maximum length of the ﬁeld is 1000 characters";
        list.add(comment)
        return "Success!!" //if
    }

    @get:GetMapping("/getComments")   //the front get the list to show on the website
    val comment: List<Any>
        get() = list
}