package com.example.ITMOhw3

import com.example.ITMOhw3.Comment
import org.springframework.http.HttpStatus
import org.springframework.web.bind.annotation.*
import java.util.*
import org.springframework.http.ResponseEntity

@CrossOrigin
@RestController
@RequestMapping("/api")
class CommentController {
    private val list: MutableList<Comment> = LinkedList<Comment>()

    @PostMapping("/addComment") //addComments
    fun addComment(@RequestBody comment: Comment): ResponseEntity<Any> {
        if (comment.getAuthor()?.length!! < 1 || comment.getAuthor()!!.length > 100)  return ResponseEntity<Any>(HttpStatus.BAD_REQUEST) //author - required no empty, maximum length is 100 characters
        if (comment.getContent()!!.length < 1 || comment.getContent()!!.length > 1000) return ResponseEntity(HttpStatus.BAD_REQUEST) //comment - required no empty, the maximum length is 1000 characters
        list.add(comment)
        return ResponseEntity(HttpStatus.ACCEPTED) //if success return accepted
    }

    @get:GetMapping("/getComments")   //the front get the list to show on the website
    val comment: List<Any>
        get() = list
}