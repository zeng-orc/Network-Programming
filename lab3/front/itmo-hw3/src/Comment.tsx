import React,{Fragment,Component, ReactNode} from 'react'
import Button from '@material-ui/core/Button';
import Grid from '@material-ui/core/Grid';
import TextField from '@material-ui/core/TextField';


const useStyles = {
    margin: '15px',
}

// export interface userstate{
//     newAuthorContent : string,
//     newCommentContent : string,
//     author:string,
//     comment:string,
//     addList:(newAuthor:any,newComment:any) =>any

// }


class Commment extends Component<any,any>{
    constructor(props:any){
        super(props)
        this.state ={
            newAuthorContent : "",
            newCommentContent : ""
        }
        
    }


    render():ReactNode{

        return(
            <Fragment >
                <Grid container spacing={3} >
                    <Grid item  xs = {12}/>
                    <Grid item xs={12}>
                        <TextField
                            required
                            id = "newAuthor" 
                            label="Enter Author"
                            variant="outlined"
                            onChange={this.inputChangeAuthor.bind(this)}
                            ></TextField>
                    </Grid>
                    <Grid  item xs={12}>
                        <TextField
                            id = "newComment"
                            label="Enter Comment"
                            variant="outlined"
                            onChange={this.inputChangeComment.bind(this)}
                        />
                    </Grid>
                    <Grid  item xs={12} >
                    <Button 
                            style = {useStyles} 
                            variant="contained" 
                            color="secondary"
                            onClick={this.handleClick}>
                            Comment
                        </Button>
                    </Grid>
                </Grid>
            </Fragment>
            
        )
    }
   

    inputChangeAuthor(e:any){
        this.setState({
            newAuthorContent:e.target.value
        })
    }
    inputChangeComment(e:any){
        this.setState({
            newCommentContent:e.target.value
        })
    }
    
     handleClick=()=>{
         this.props.addList(this.state.newAuthorContent,this.state.newCommentContent)
     }
}

export default Commment;